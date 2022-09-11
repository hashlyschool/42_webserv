
#include "../inc/Parser.hpp"

const ft::ParserException exception_ToManyArgError("Too many input arguments, only conf file path for the webserver is necessary!");
const ft::ParserException exception_ReadConfFileError("Error reading server configuration file!");
const ft::ParserException exception_NoServerError("No server found in the conf file!");
const ft::ParserException exception_NameParseError("Found unknown directive in the conf file or incorrect sintax!");
const ft::ParserException exeption_ServerParamError("Did not find either 'listen' or 'server_name' parameter in the conf file!");
const ft::ParserException exception_ServerRedundancyError("There are two servers serving at the same IP address/port!");
const ft::ParserException exception_IndexOutOfRangeError("Index is out of range!");

ft::Parser::Parser(std::string pathConf) : _pathConf(pathConf)
{
	_parse_server_config(_pathConf);
	// базовая информация из конфигураций серверов
	std::cout << "Successfully parsed " << getConfigServers().size() << " server configuration" << std::endl;
	for (size_t i = 0; i < getConfigServers().size(); i++)
	{
		std::cout << std::endl << i + 1 << " - server:" << std::endl;
		struct in_addr paddr;
		paddr.s_addr = getConfigServers().at(i).getHost();
		std::cout << "Host - " << inet_ntoa(paddr) << std::endl;
		std::cout << "Port - " << ntohs(getConfigServers().at(i).getPort()) << std::endl;
		std::cout << "ServerName - " << getConfigServers().at(i).getServerName() << std::endl;
		std::cout << "Root - " << getConfigServers().at(i).getRoot() << std::endl;
	}

	std::cout << "\n\nStart test searching of locations:" << std::endl;
	// ConfigServer serv = getConfigServers().at(0);
	// std::string url = "/images/about";
	// std::cout << "Test case: url = " + url << std::endl;
	// const ft::ALocation *loc = serv.getLocation(url);
	// if (loc == NULL)
	// {
	// 	std::cout << "Location is not found!" << std::endl;
	// }
	// else if (loc->getUrl() == url || loc->getUrl() == (url+"/"))
	// 	std::cout << "Test - OK!" << std::endl;
	// else
	// 	std::cout << "Test - Fail!" << std::endl;
	// std::cout << "found - " + loc->getUrl() << std::endl;
	ConfigServer serv = getConfigServers().at(3);
	std::cout << serv.getServerName() << std::endl;
	std::string url = "/cgi-bin/";
	const ft::ALocation *loc = serv.getLocation(url);
	if (loc == NULL)
		std::cout << "Location is not found!" << std::endl;
	else
	{
		std::string filename = serv.getFilename(url, *loc);
		std::cout << "found - " + filename << std::endl;
	}
}

ft::Parser::~Parser()
{
}

const std::vector<ft::ConfigServer> &ft::Parser::getConfigServers() const
{
	return (_servers);
}

// std::string ft::Parser::_verify_input(int ac, char **av)
// {
// 	if (ac > 2)
// 		throw exception_ToManyArgError;
// 	if (ac == 2)
// 		return av[1];
// 	else
// 		return default_config_file;
// }

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
		if (parsed_line[0] == "server" && parsed_line[1] == "{" && parsed_line.size() == 2) // обернуть в проверку начала контекста server
		{
			brackets = 1;
			_parse_server_block(iss, brackets);
		}
		else
		{
			std::string line;
			for (size_t i = 0; i < parsed_line.size(); i++)
				line = parsed_line[i] + " ";
			throw std::invalid_argument("Parser error: wrong format - " + line);
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

bool ft::Parser::_server_param_check(const std::map<std::string, std::vector<std::string> > &param)
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

			std::vector<std::string> args;
			for (size_t i = 1; i < parsed_line.size(); i++)
				args.push_back(parsed_line[i]);
			if (_check_name(parsed_line[0]) == server_error_page)
			{
				if (args.size() != 2 || args[0].empty() || args[1].empty())
					throw std::invalid_argument("Parser error: wrong root error page format");
				serverBlock.server_err_page.insert(err_type(atoi(args[0].c_str()), args[1]));
			}
			else
				serverBlock.server_param.insert(dir_type(parsed_line[0], args));
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
	_erase_semicolon(parsed_line); // удаляет ";" в конце строчки
	switch (_check_name(parsed_line[0]))
	{
	case -1:
		throw exception_NameParseError;
	case server_error_page:
		if (!_validKeys(parsed_line[1], serverBlock, true))
			throw std::invalid_argument("Parser error: incorrect key of error rule in the config");
		break;
	default:
		if (!_validKeys(parsed_line[0], serverBlock, false))
			throw std::invalid_argument("Parser error: incorrect key of regular rule in the config");
		break;
	}
}

bool ft::Parser::_validKeys(std::string &key, serverBlockConfig_t &serverBlock, bool isError)
{
	if (isError == true && serverBlock.server_err_page.size() > 0)
	{
		std::map<int, std::string>::iterator it = serverBlock.server_err_page.find(atoi(key.c_str()));
		if (it != serverBlock.server_err_page.end() && it->first == atoi(key.c_str()))
			return false;
	}
	else if (serverBlock.server_param.size() > 0 && key.empty() == false)
	{
		std::map<std::string, std::vector<std::string> >::iterator it = serverBlock.server_param.find(key);
		if (it != serverBlock.server_param.end() &&	it->first == key)
			return false;
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

	if (value.size() > 2)
		throw std::invalid_argument("Parser error: invalid error page info");
	error.first = _checkPortVal(value[0]);
	error.second = value[1];
	return error;
}

void ft::Parser::_setServerInfo(ssize_t index, serverBlockConfig_t &serverBlock)
{
	for (std::map<std::string, std::vector<std::string> >::iterator it = serverBlock.server_param.begin(); it != serverBlock.server_param.end(); it++)
		_fillConfig(index, _check_name(it->first), it->second);
	for (std::map<int, std::string>::iterator it = serverBlock.server_err_page.begin(); it != serverBlock.server_err_page.end(); it++)
		_fillRootErrorPages(*it, index);
	_servers[index].setLocations(serverBlock.server_locations);
}

void ft::Parser::_fillConfig(ssize_t index, size_t key, std::vector<std::string> value)
{
	switch (key)
	{
	case server_listen:
		_fillHostPort(value, index);
		break;
	case server_server_name:
		_fillServerName(value, index);
		break;
	case server_autoindex:
		_fillAutoindex(value, index);
		break;
	case server_root:
		_fillServerRoot(value, index);
		break;
	case server_index:
		_fillIndex(value, index);
		break;
	case server_allowed_methods:
		_fillRootMethods(value, index);
		break;
	case server_max_body_size:
		_fillRootMaxBodySize(value, index);
		break;
	case server_upload_path:
		_fillUploadPath(value, index);
		break;
	}
}

void ft::Parser::_fillLocation(ft::Location &obj, std::string key, std::vector<std::string> args)
{
	switch (_check_name(key))
	{
	case server_allowed_methods:
		_fillLocationMethods(args, obj);
		break;
	case server_root:
		_fillLocationRoot(args, obj);
		break;
	case server_redirection:
		_fillLocationRedirection(args, obj);
		break;
	case server_error_page:
		_fillLocationErrorsPages(args, obj);
		break;
	case server_index:
		_fillLocationIndex(args, obj);
		break;
	case server_autoindex:
		_fillLocationAutoindex(args, obj);
		break;
	case server_upload_path:
		_fillLocationUploadPath(args, obj);
		break;
	case server_bin_path_py:
		_fillLocationBinPathPy(args, obj);
		break;
	case server_bin_path_sh:
		_fillLocationBinPathSh(args, obj);
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

void ft::Parser::_fillLocationMethods(std::vector<std::string> args, ft::Location &location)
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

void ft::Parser::_fillLocationRoot(std::vector<std::string> args, ft::Location &location)
{
	if (!location.getRoot().empty() || args.size() != 1)
		throw std::invalid_argument("Parser error: root location error");
	args[0] = _checkSyntaxPath(args[0]);
	location.setRoot(args[0]);
}

void ft::Parser::_fillLocationRedirection(std::vector<std::string> args, ft::Location &location)
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

void ft::Parser::_fillLocationErrorsPages(std::vector<std::string> args, ft::Location &location)
{
	std::pair<int, std::string> str;
	str.first = _fillErrorPage(args).first;
	str.second = _fillErrorPage(args).second;
	if (args.size() < 2 || !str.first || str.second.empty())
		throw std::invalid_argument("Parser error: wrong location error page format");
	location.setErrorPages(str.first, str.second);
}

void ft::Parser::_fillLocationIndex(std::vector<std::string> args, ft::Location &location)
{
	if (!location.getIndex().empty() || args.size() != 1)
		throw std::invalid_argument("Parser error: wrong lcoation index page");
	location.setIndex(args[0]);
}

void ft::Parser::_fillLocationAutoindex(std::vector<std::string> args, ft::Location &location)
{
	if (args.size() != 1 || (args[0] != "on" && args[0] != "off"))
		throw std::invalid_argument("Parser error: location Autoindex error");
	else if (args[0] == "on")
		location.setAutoIndex(true);
}

void ft::Parser::_fillLocationUploadPath(std::vector<std::string> args, ft::Location &location)
{
	if (!location.getUploadPath().empty() || args.size() != 1)
		throw std::invalid_argument("Parser error: wrong location directory to upload");
	args[0] = _checkSyntaxPath(args[0]);
	location.setUploadPath(args[0]);
}

void ft::Parser::_fillLocationBinPathPy(std::vector<std::string> args, ft::Location &location)
{
	if (!location.getBinPathPy().empty() || args.size() != 1 || !location.getIsCgi())
		throw std::invalid_argument("Parser error: wrong location bin path");
	args[0] = _checkSyntaxPath(args[0]);
	location.setBinPathPy(args[0]);
}

void ft::Parser::_fillLocationBinPathSh(std::vector<std::string> args, ft::Location &location)
{
	if (!location.getBinPathSh().empty() || args.size() != 1 || !location.getIsCgi())
		throw std::invalid_argument("Parser error: wrong location bin path");
	args[0] = _checkSyntaxPath(args[0]);
	location.setBinPathSh(args[0]);
}

void ft::Parser::_fillHostPort(std::vector<std::string> value, ssize_t index)
{
	std::vector<std::string> vals;

	if (_servers[index].getHost() != 0 || _servers[index].getPort() || value.size() != 1)
		throw std::invalid_argument("Parser error: host/port error");
	vals = _getHostPort(value[0]);
	if (vals.size() > 1)
	{
		_servers[index].setHost(vals[0]);
		_servers[index].setPort(_checkPortVal(vals[1]));
	}
	else
	{
		if (!_isHost(vals[0]))
			_servers[index].setPort(_checkPortVal(vals[0]));
		else
			_servers[index].setPort(80);
	}
}

void ft::Parser::_fillServerName(std::vector<std::string> value, ssize_t index)
{
	if (!_servers[index].getServerName().empty() || value.size() != 1)
		throw std::invalid_argument("Parser error: server name error");
	_servers[index].setServerName(value[0]);
}

void ft::Parser::_fillAutoindex(std::vector<std::string> value, ssize_t index)
{
	if (value.size() != 1 || (value[0] != "on" && value[0] != "off"))
		throw std::invalid_argument("Parser error: root autoindex error");
	if (value[0] == "on")
		_servers[index].setAutoIndex(true);
}

void ft::Parser::_fillServerRoot(std::vector<std::string> value, ssize_t index)
{
	if (!_servers[index].getRoot().empty() || value.size() != 1)
		throw std::invalid_argument("Parser error: root error");
	value[0] = _checkSyntaxPath(value[0]);
	_servers[index].setRoot(value[0]);
}

void ft::Parser::_fillIndex(std::vector<std::string> value, ssize_t index)
{
	if (!_servers[index].getIndex().empty() || value.size() != 1)
		throw std::invalid_argument("Parser error: root index page error");
	_servers[index].setIndex(value[0]);
}

void ft::Parser::_fillRootMethods(std::vector<std::string> value, ssize_t index)
{
	if (value.size() < 1 || value.size() > 3)
		throw std::invalid_argument("Parser error: root methods error");
	for (size_t i = 0; i < value.size(); i++)
	{
		if (value[i] == "GET")
			_servers[index].setIsGet(true);
		else if (value[i] == "POST")
			_servers[index].setIsPost(true);
		else if (value[i] == "DELETE")
			_servers[index].setIsDelete(true);
		else
			throw std::invalid_argument("Parser error: wrong root method");
	}
}

void ft::Parser::_fillRootMaxBodySize(std::vector<std::string> value, ssize_t index)
{
	if (_servers[index].getMaxBodySize() != 0 || value.size() != 1)
		throw std::invalid_argument("Parser error: root max body size error");
	if (value[0][value[0].size() - 1] == 'M') {
		value[0] = value[0].substr(0, value[0].size() - 1);
		_servers[index].setMaxBodySize(_checkPortVal(value[0]) * 1024 * 1024);
	} else if (isdigit(value[0][value[0].size() - 1])) {
		_servers[index].setMaxBodySize(_checkPortVal(value[0]) * 1024);
	} else {
		throw std::invalid_argument("Parser error: wrong value root MaxBodySize");
	}
}

void ft::Parser::_fillUploadPath(std::vector<std::string> value, ssize_t index)
{
	if (!_servers[index].getUploadPath().empty() || value.size() != 1)
		throw std::invalid_argument("Parser error: root directory to upload error");
	value[0] = _checkSyntaxPath(value[0]);
	_servers[index].setUploadPath(value[0]);
}

void ft::Parser::_fillRootErrorPages(err_type value, ssize_t index)
{
	if (!value.first || value.second.empty())
		throw std::invalid_argument("Parser error: wrong root error page format");
	_servers[index].setErrorPages(value.first, value.second);
}

std::vector<std::string> ft::Parser::_getHostPort(std::string host)
{
	size_t i = 0;
	size_t j = 0;
	size_t dot = 0;
	std::vector<std::string> hostPort;

	while (host[i])
	{
		if (isdigit(host[i]))
			i++;
		else if (host[i] == '.')
		{
			if (j > 3 || dot == 3 || j < 1 || !(atoi(host.substr(i - j, i).c_str()) >= 0 && atoi(host.substr(i - j, i).c_str()) <= 255))
				throw std::invalid_argument("Parser error: wrong host format: " + host);
			j = -1;
			dot++;
			i++;
		}
		else if (host[i] == ':' && i && j > 0)
		{
			hostPort.push_back(host.substr(0, i));
			hostPort.push_back(host.substr(i + 1, host.size()));
			return hostPort;
		}
		else
			throw std::invalid_argument("Parser error: wrong number: " + host);
		j++;
	}
	if (!hostPort.size())
		hostPort.push_back(host.substr(0, i));
	else
		throw std::invalid_argument("Parser error: wrong host/port");
	return hostPort;
}

bool ft::Parser::_isHost(std::string value)
{
	size_t pos = value.find(".");

	if (pos != std::string::npos)
		return true;
	return false;
}

std::string &ft::Parser::_checkSyntaxPath(std::string &_path)
{
	if (!_path.empty())
	{
		if (_path[0] != '/')
			_path = "/" + _path;
		if (_path[_path.size() - 1] == '/')
			_path.erase(_path.size() - 1, 1);
	}
	return (_path);
}
