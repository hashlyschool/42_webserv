#include "../inc/Parser.hpp"

ft::Parser::Parser(std::string pathConf)
{
	parseConfig(pathConf);
}

ft::Parser::~Parser()
{
	std::vector<ConfigServer *>::iterator	it;

	for(it = _configServers.begin(); it !=_configServers.end(); ++it)
	{
		std::cout << "delete ConfigServer in Parser" << std::endl;
		delete *it;
	}

}

ft::t_serverConf ft::Parser::parseOneServer()
{
	t_serverConf	temp;

	static int	i = 1;
	temp._host = "127.0.0.1";
	if (i == 1)
		temp._port = "8080";
	else
		temp._port = "8081";
	temp._serverName = "hello";
	temp._maxBodySize = 100;
	temp._locations.clear();
	i++;
	return (temp);
}

void ft::Parser::parseConfig(std::string pathConf)
{
	std::cout << "Parse config: " << pathConf << std::endl;
	int				numServers = 2;
	t_serverConf	temp;

	for (int i = 0; i < numServers; ++i)
	{
		temp = parseOneServer();
		_configServers.push_back(new ConfigServer(temp));
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
