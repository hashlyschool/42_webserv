
#include "../inc/Parser.hpp"

ft::Parser::Parser(std::string conf)
{
	std::ifstream _fd;
	_fd.open(conf, std::ios::in);
	if (_fd.is_open() == false)
		std::cerr << "Error can't open file" << std::endl;
	try
	{
		char buf[10];
		while (_fd.get(buf, 10, '\0'))
			_config += buf;
	}
	catch (std::ifstream::failure e) { std::cerr << "Ifstream failure exception" << std::endl; }
	_fd.close();
	Parse();
}

void ft::Parser::Parse() {
	std::vector<std::string> token;
	checkBrackets();
	while (!_config.empty())
		token.push_back(Split(_config, "\n"));
	for (std::vector<std::string>::iterator it = token.begin(); it != token.end(); it++)
		std::cout << *it << "\n";

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
//	if (bracket != 0)
	//	throw(std::exception());
}

std::string ft::Parser::Split(std::string &line, std::string delimiter)
{
	size_t pos = 0;
	std::string token;
	pos = line.find(delimiter);
	if (pos == std::string::npos)
		return (0);

    token = line.substr(0, pos);
    line.erase(0, pos + delimiter.length());
	return (token);
}

ft::Parser::~Parser() {

}

int	ft::Parser::getNumServers() const
{
	return (_configServers.size());
}

const ft::ConfigServer	*ft::Parser::getConfigServer(int index) const
{
	return (_configServers.at(index));
}
