#include "../inc/ConfigServer.hpp"

// ft::ConfigServer::ConfigServer(std::map<std::string, std::string> param, std::map<int, std::string> err_page, std::vector<Location> loc) : server_directives(param), server_error_page_location(err_page), server_locations(loc)
// {
// 	// The standard form is "listen  127.0.0.1:8080"
// 	// We assume that our servers listen on all ip address, i.e., "8080" means "0.0.0.0:8080"
// 	std::string listen_param = param.find("listen")->second;
// 	size_t seperator_pos = listen_param.find(':');
// 	std::string ipPart = listen_param.substr(0, seperator_pos);
// 	std::string portPart = listen_param.substr(seperator_pos + 1);
// 	_host = (seperator_pos != std::string::npos) ? ipPart : "0.0.0.0";
// 	_port = (seperator_pos != std::string::npos) ? atoi(portPart.c_str()) : atoi(listen_param.c_str());
// }

ft::ConfigServer::ConfigServer()
{
	_port = 0;
	_host = htonl(INADDR_ANY);
	_serverName = "";
	_maxBodySize = 0;
}

ft::ConfigServer &ft::ConfigServer::operator=(const ConfigServer &other)
{
	ALocation::operator=(other);
	_port = other._port;
	_host = other._host;
	_serverName = other._serverName;
	_maxBodySize = other._maxBodySize;
	_locations.insert(_locations.end(), other._locations.begin(), other._locations.end());
	return *this;
}

ft::ConfigServer::~ConfigServer()
{
	//Нужно очистить выделенную память для _location's
}

void ft::ConfigServer::setPort(const int &port)
{
	_port = htons(port);
}

const u_short &ft::ConfigServer::getPort() const
{
	return _port;
}

void ft::ConfigServer::setHost(const std::string &host)
{
	_host = inet_addr(host.c_str());
}

const in_addr_t &ft::ConfigServer::getHost() const
{
	return _host;
}

void ft::ConfigServer::setServerName(const std::string &serverName)
{
	_serverName = serverName;
}

const std::string &ft::ConfigServer::getServerName() const
{
	return _serverName;
}

void ft::ConfigServer::setMaxBodySize(const int &maxBodySize)
{
	_maxBodySize = maxBodySize;
}

const int &ft::ConfigServer::getMaxBodySize() const
{
	return _maxBodySize;
}

std::vector<ft::Location> &ft::ConfigServer::getLocations()
{
	return _locations;
}
