
#include "../inc/Parser.hpp"

ft::Parser::Parser(std::string conf)
{
	std::ifstream _fd;
	_fd.open(conf.c_str(), std::ios::in);
	if (_fd.is_open() == false)
		std::cerr << "Error can't open file" << std::endl;
//	try
//	{
		std::string buf;
		while (std::getline(_fd, buf))
		{
			buf = trim(buf, " \t");
			if (buf.empty() || buf[0] == '#')
				continue;
			_config += buf;
			_config += "\n";
		}
//	}
//	catch (std::ifstream::failure e) { std::cerr << "Ifstream failure exception" << std::endl; }
	_fd.close();
	Parse();
}

inline std::string ft::Parser::trim( std::string line, std::string trimmer)
{
	line.erase(line.find_last_not_of(trimmer)+1);         //suffixing spaces
    line.erase(0, line.find_first_not_of(trimmer));       //prefixing spaces
    return line;
}

void ft::Parser::Parse() {
	std::vector<std::string> token;
	std::string tmp;
	checkBrackets();
	while (!_config.empty())
	{
		tmp = Split(_config, "SERVER");
		if (tmp.empty() == false)
			token.push_back(tmp);
	}
//	for (std::vector<std::string>::iterator it = token.begin(); it != token.end(); it++)
//		std::cout << *it << "\n";
	size_t i = token.size();
	std::cout << i << " tokens\n";
	while (i--)
		_configServers.push_back(new ConfigServer((parseOneServer(token[i]))));
}

ft::t_serverConf				ft::Parser::parseOneServer(std::string token)
{
	t_serverConf config;
	std::string tmp;
	config._port = "";
	config._host = "";
	config._serverName = "hi";
	config._maxBodySize = 100;
	while (!token.empty())
	{
		tmp = Split(token, "\n");
		if (tmp.find("listen") != std::string::npos)
		{
			std::cout << tmp << "\n";
			tmp = tmp.substr(7, 14);
			std::cout << tmp << "\n";
			config._host = tmp.substr(0, 9);
			config._port = tmp.substr(10, 12);
			std::cout << config._host << std::endl;
			std::cout << config._port << std::endl;
		}
	}
	return config;
}
void ft::Parser::checkBrackets()
{
	std::string::iterator begin = _config.begin();
	size_t bracket = 0;
	while (begin != _config.end())
	{
		if (*begin == '{' && !bracket)
			bracket += 1;
	//	else if (*begin == '}' && !bracket)
		//	throw(std::exception());//todo  make parse exception
		else if (*begin == '}' && bracket)
			bracket -= 1;
		begin++;
	}
	//if (bracket != 0)
	//	throw(std::exception());
}

std::string ft::Parser::Split(std::string &line, std::string delimiter)
{
	size_t pos = 0;
	std::string token;
	pos = line.find(delimiter);
	if (pos == std::string::npos)
	{
		token = line;
		line.erase();
		return (trim(token, " \t"));
	}

    token = line.substr(0, pos);
    line.erase(0, pos + delimiter.length());
	return (trim(token, " \t"));
}

ft::Parser::~Parser() 
{
	std::vector<ConfigServer *>::iterator	it = _configServers.begin();

	while (it != _configServers.end())
	{
		std::cout << "Virtual Server deleted\n";
		delete *it++;
	}
}

int	ft::Parser::getNumServers() const
{
	return (_configServers.size());
}

const ft::ConfigServer	*ft::Parser::getConfigServer(int index) const
{
	return (_configServers.at(index));
}
