
#include "../inc/Parser.hpp"

const ft::ParserException exception_ToManyArgError("Too many input arguments, only conf file path for the webserver is necessary!");
const ft::ParserException exception_ReadConfFileError("Error reading server configuration file!");
const ft::ParserException exception_NoServerError("No server found in the conf file!");
const ft::ParserException exception_NameParseError("Found unknown directive in the conf file!");
const ft::ParserException exeption_ServerParamError("Did not find either 'listen' or 'server_name' parameter in the conf file!");
const ft::ParserException exception_ServerRedundancyError("There are two servers serving at the same IP address/port!");
const ft::ParserException exception_IndexOutOfRangeError("Index is out of range!");

ft::Parser::Parser(std::string pathConf) : _pathConf(pathConf)
{
	_parse_server_config(_pathConf);
}

ft::Parser::~Parser()
{
}

const std::vector<ft::ConfigServer> &ft::Parser::getConfigServers() const
{
	return (_servers);
}

std::string ft::Parser::_verify_input(int ac, char **av)
{
	if (ac > 2)
		throw exception_ToManyArgError;
	if (ac == 2)
		return av[1];
	else
		return default_config_file;
}

void ft::Parser::_parse_server_config(const std::string &conf_file)
{
	std::vector<std::string> parsed_line;

	int brackets = 0;

	// Читаем файл в строку
	if (!read_file(conf_file, _raw_config_file))
		throw exception_ReadConfFileError;

	std::istringstream iss(_raw_config_file);
	while (!iss.eof())
	{
		// Проходим по каждой строке файла, разбивая ее на токены
		split_next_line(iss, parsed_line);
		// Проверка на пустую строку или комментарий
		if (parsed_line.size() == 0 || parsed_line[0][0] == '#')
			continue;
		// Если нашли директиву Server
		if (parsed_line[0] == "server" && parsed_line[1] == "{") // обернуть в проверку начала контекста server
		{
			brackets = 1;
			_parse_server_block(iss, brackets);
		}
	}
	if (_servers.size() == 0)
		throw exception_NoServerError;
}

bool ft::Parser::read_file(const std::string &file_name, std::string &raw_record)
{
	std::ostringstream content;

	std::ifstream ifs(file_name.c_str(), std::ifstream::in); // open file
	if (ifs.good())
	{
		content << ifs.rdbuf();
		ifs.close();
		raw_record = content.str();
		return (true);
	}
	return (false);
}

void ft::Parser::split_next_line(std::istringstream &input_stream, std::vector<std::string> &output_vector)
{
	output_vector.clear();
	std::string line;
	std::getline(input_stream, line);

	if (line.find('#'))
		line = line.substr(0, line.find('#'));

	std::istringstream issline(line);
	std::istream_iterator<std::string> last;
	std::istream_iterator<std::string> first(issline);

	std::vector<std::string> parsed(first, last);
	output_vector = parsed;
}

int ft::Parser::_check_name(const std::string &name)
{
	if (name == "server_name")
		return server_server_name;
	if (name == "listen")
		return server_listen;
	if (name == "root")
		return server_root;
	if (name == "index")
		return server_index;
	if (name == "autoindex")
		return server_autoindex;
	if (name == "location")
		return server_location;
	if (name == "error_page")
		return server_error_page;
	if (name == "allowed_methods")
		return server_allowed_methods;
	if (name == "client_max_body_size")
		return server_max_body_size;
	if (name == "upload_path")
		return server_upload_path;
	if (name == "bin_path_py")
		return server_bin_path_py;
	if (name == "bin_path_sh")
		return server_bin_path_sh;
	if (name == "redirection")
		return server_redirection;
	return -1;
}

bool ft::Parser::_server_param_check(const std::map<std::string, std::string> &param)
{
	if (param.find("listen") != param.end() && param.find("server_name") != param.end())
		return true;
	else
		return false;
}

void ft::Parser::_parse_server_block(std::istringstream &iss, int &brackets)
{
	serverBlockConfig_t serverBlock;
	std::vector<std::string> parsed_line;

	while (brackets > 0)
	{
		split_next_line(iss, parsed_line);
		if (parsed_line.size() == 0 || parsed_line[0][0] == '#')
			continue;
		// Если конец блока
		else if (parsed_line[0] == "}")
		{
			if (!_server_param_check(serverBlock.server_param))
				throw exeption_ServerParamError;
			_addServer(serverBlock);
			brackets = 0;
			return;
		}
		// Если начало блока директивы server
		else if (_check_name(parsed_line[0]) == server_location && parsed_line.size() == 3 &&
				 parsed_line[2] == "{")
		{
			brackets = 2;
			Location one_location;
			_fillLocationName(one_location, parsed_line[1]);
			while (brackets > 1)
			{
				split_next_line(iss, parsed_line);
				if (parsed_line.size() == 0 || parsed_line[0][0] == '#')
					continue;
				if (parsed_line[0] == "}")
				{
					serverBlock.server_locations.push_back(one_location);
					brackets = 1;
					continue;
				}
				else
				{
					if (_check_name(parsed_line[0]) == -1) // проверка существования директивы
						throw exception_NameParseError;
					_erase_semicolon(parsed_line);

					std::vector<std::string> args;
					for (size_t i = 1; i < parsed_line.size(); i++)
						args.push_back(parsed_line[i]);
					_fillLocation(one_location, parsed_line[0], args);
				}
			}
		}
		// иначе обработка директив внутри контекста
		else
		{
			_checkDirective(parsed_line, serverBlock); // проверка на корректность правила

			if (_check_name(parsed_line[0]) == server_error_page)
				serverBlock.server_err_page.insert(err_type(atoi(parsed_line[1].c_str()), parsed_line[2]));
			else
				serverBlock.server_param.insert(dir_type(parsed_line[0], parsed_line[1]));
		}
	}
}

void ft::Parser::_addServer(serverBlockConfig_t &serverBlock)
{
	_servers.push_back(ConfigServer());
	_setServerInfo(_servers.size() - 1, serverBlock);
}

void ft::Parser::_erase_semicolon(std::vector<std::string> &parsed_line)
{
	if (parsed_line[parsed_line.size() - 1][parsed_line[parsed_line.size() - 1].size() - 1] == ';')
		parsed_line[parsed_line.size() - 1].erase(parsed_line[parsed_line.size() - 1].size() - 1);
	else
		std::invalid_argument("Parser error: directive string does not close by semilicon");
}

void ft::Parser::_erase_comma(std::vector<std::string> &parsed_line, size_t &i)
{
	if (parsed_line[i][parsed_line[i].size() - 1] == ',')
		parsed_line[i].erase(parsed_line[i].size() - 1);
}

void ft::Parser::_checkDirective(std::vector<std::string> &parsed_line, serverBlockConfig_t &serverBlock)
{
	_erase_semicolon(parsed_line); // удалить ";" в конце строчки
	switch (_check_name(parsed_line[0]))
	{
	case -1:
		throw exception_NameParseError;
	case server_error_page:
		if (!_validKeys(parsed_line[1], serverBlock, server_error_page))
			throw std::invalid_argument("Parser error: incorrect key of error rule in the config");
		break;
	default:
		if (!_validKeys(parsed_line[0], serverBlock, 0))
			throw std::invalid_argument("Parser error: incorrect key of regular rule in the config");
		break;
	}
}

bool ft::Parser::_validKeys(std::string &key, serverBlockConfig_t &serverBlock, size_t code)
{
	switch (code)
	{
	case server_error_page:
		if (serverBlock.server_err_page.find(atoi(key.c_str()))->first == atoi(key.c_str()))
			return false;
		break;
	default:
		if (serverBlock.server_param.find(key)->first == key)
			return false;
		break;
	}
	return true;
}

int ft::Parser::_checkPortVal(std::string str)
{
	int val = 0;

	for (size_t i = 0; i < str.size(); i++)
	{
		if (!isdigit(str[i]))
			throw std::invalid_argument("Parser error: not digit");
	}
	val = static_cast<int>(strtod(str.c_str(), 0));
	if (val < 0 or val > 65535)
		throw std::invalid_argument("Parser error: number out of range");
	return val;
}

ft::Parser::err_type ft::Parser::_fillErrorPage(std::vector<std::string> value)
{
	std::pair<int, std::string> error;

	error.first = _checkPortVal(value[0]);
	error.second = value[1];
	if (value.size() > 2)
		throw std::invalid_argument("Parser error: invalid error page info");
	return error;
}

void ft::Parser::_setServerInfo(ssize_t index, serverBlockConfig_t &serverBlock)
{
	// TMP
	// serverBlock.server_param.begin();
	// _servers[index].setServerName("localhost");
}

void ft::Parser::_fillLocation(ft::Location &obj, std::string key, std::vector<std::string> args)
{
	switch (_check_name(key))
	{
	case server_allowed_methods:
		_fillLocationMethods(key, args, obj);
		break;
	case server_root:
		_fillLocationRoot(key, args, obj);
		break;
	case server_redirection:
		_fillLocationRedirection(key, args, obj);
		break;
	case server_error_page:
		_fillLocationErrorsPages(key, args, obj);
		break;
	case server_index:
		_fillLocationIndex(key, args, obj);
		break;
	case server_autoindex:
		_fillLocationAutoindex(key, args, obj);
		break;
	case server_upload_path:
		_fillLocationUploadPath(key, args, obj);
		break;
	case server_bin_path_py:
		_fillLocationBinPathPy(key, args, obj);
		break;
	case server_bin_path_sh:
		_fillLocationBinPathSh(key, args, obj);
		break;
	}
}

void ft::Parser::_fillLocationName(ft::Location &obj, std::string line)
{
	if (!obj.getUrl().empty())
		throw std::invalid_argument("Parser error: location error");

	obj.setUrl(line);
	if (line == "/cgi-bin/")
		obj.setIsCgi(true);
}

void ft::Parser::_fillLocationMethods(std::string key, std::vector<std::string> args, ft::Location &location)
{
	for (size_t i = 0; i < args.size(); i++)
	{
		if (args[i].size() < 1 || args[i].size() > 6)
			throw std::invalid_argument("Parser error: location methods error");
		if (args[i] == "GET")
			location.setIsGet(true);
		else if (args[i] == "POST")
			location.setIsPost(true);
		else if (args[i] == "DELETE")
			location.setIsDelete(true);
		else
			throw std::invalid_argument("Parser error: wrong location method");
	}
}

void ft::Parser::_fillLocationRoot(std::string key, std::vector<std::string> args, ft::Location &location)
{
	if (!location.getRoot().empty() || args.size() != 1)
		throw std::invalid_argument("Parser error: root location error");
	location.setRoot(args[0]);
}

void ft::Parser::_fillLocationRedirection(std::string key, std::vector<std::string> args, ft::Location &location)
{
	if (location.getIsRedirect() || args.size() != 2 || location.getIsCgi())
		throw std::invalid_argument("Parser error: location redirection error");
	location.setIsRedirect(true);
	location.setIndex(args[0]);
	int code = _checkPortVal(args[1]);
	if (code != 302)
		throw std::invalid_argument("Parser error: wrong number, you can use only code 302");
	location.setRedirectionCode(code);
}

void ft::Parser::_fillLocationErrorsPages(std::string key, std::vector<std::string> args, ft::Location &location)
{
	std::pair<int, std::string> str;
	str.first = _fillErrorPage(args).first;
	str.second = _fillErrorPage(args).second;
	if (args.size() < 2 || !str.first || str.second.empty())
		throw std::invalid_argument("Parser error: wrong location error page format");
	location.setErrorPages(str.first, str.second);
}

void ft::Parser::_fillLocationIndex(std::string key, std::vector<std::string> args, ft::Location &location)
{
	if (!location.getIndex().empty() || args.size() != 1)
		throw std::invalid_argument("Parser error: wrong lcoation index page");
	location.setIndex(args[0]);
}

void ft::Parser::_fillLocationAutoindex(std::string key, std::vector<std::string> args, ft::Location &location)
{
	if (args.size() != 1 || (args[0] != "on" && args[0] != "off"))
		throw std::invalid_argument("Parser error: location Autoindex error");
	else if (args[0] == "on")
		location.setAutoIndex(true);
}

void ft::Parser::_fillLocationUploadPath(std::string key, std::vector<std::string> args, ft::Location &location)
{
	if (!location.getUploadPath().empty() || args.size() != 1)
		throw std::invalid_argument("Parser error: wrong location directory to upload");
	location.setUploadPath(args[0]);
}

void ft::Parser::_fillLocationBinPathPy(std::string key, std::vector<std::string> args, ft::Location &location)
{
	if (!location.getBinPathPy().empty() || args.size() != 1 || !location.getIsCgi())
		throw std::invalid_argument("Parser error: wrong location bin path");
	location.setBinPathPy(args[0]);
}

void ft::Parser::_fillLocationBinPathSh(std::string key, std::vector<std::string> args, ft::Location &location)
{
	if (!location.getBinPathSh().empty() || args.size() != 1 || !location.getIsCgi())
		throw std::invalid_argument("Parser error: wrong location bin path");
	location.setBinPathSh(args[0]);
}
