
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
			while (brackets > 0)
			{
				split_next_line(iss, parsed_line);
				if (parsed_line.size() == 0 || parsed_line[0][0] == '#')
					continue;
				_parse_server_block(iss, parsed_line, brackets);
			}
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
	return -1;
}

bool ft::Parser::_server_param_check(const std::map<std::string, std::string> &param)
{
	if (param.find("listen") != param.end() && param.find("server_name") != param.end())
		return true;
	else
		return false;
}

void ft::Parser::_parse_server_block(std::istringstream &iss, std::vector<std::string> &parsed_line, int &brackets)
{
	serverBlockConfig_t serverBlock;
	// Если конец блока
	if (parsed_line[0] == "}")
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
				{
					_erase_comma(parsed_line, i);
					args.push_back(parsed_line[i]);
				}
				_fillLocation(one_location, parsed_line[0], args);
			}
		}
	}
	// иначе обработка директив внутри контекста
	else
	{
		if (_check_name(parsed_line[0]) == -1)
			throw exception_NameParseError;
		_erase_semicolon(parsed_line); // удалить ";" в конце строчки

		// serverBlock.server_param.insert(std::pair<std::string, std::string>(parsed_line[0], parsed_line[1]));
		// if (parsed_line[0] == "error_page")
		// {
		// 	if (parsed_line[2][parsed_line[2].size() - 1] == ';')
		// 		parsed_line[2].erase(parsed_line[2].size() - 1);
		// 	serverBlock.server_err_page.insert(std::pair<int, std::string>(atoi(parsed_line[1].c_str()), parsed_line[2]));
		// }
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
}

void ft::Parser::_erase_comma(std::vector<std::string> &parsed_line, size_t &i)
{
	if (parsed_line[i][parsed_line[i].size() - 1] == ',')
		parsed_line[i].erase(parsed_line[i].size() - 1);
}

void ft::Parser::_setServerInfo(ssize_t index, serverBlockConfig_t &serverBlock)
{
	// TMP
	serverBlock.server_param.begin();
	_servers[index].setServerName("localhost");
}

void ft::Parser::_fillLocationName(ft::Location &obj, std::string line)
{
	if (!obj.getUrl().empty())
		throw std::invalid_argument("Parser error: location error");

	obj.setUrl(line);
	if (line == "/cgi-bin/")
		obj.setIsCgi(true);
}

void ft::Parser::_fillLocation(ft::Location &obj, std::string key, std::vector<std::string> args)
{
	// TMP
	if (key == "Index")
	{
		obj.setIndex(args[0]);
	}
}
