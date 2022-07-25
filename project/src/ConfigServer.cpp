#include "../inc/ConfigServer.hpp"

ft::ConfigServer::ConfigServer(t_serverConf serverConf)
{
	_port = serverConf._port;
	_host = serverConf._host;
	_serverName = serverConf._serverName;
	_maxBodySize = serverConf._maxBodySize;

	//Здесь нужно выделить память под _location's
	_locations = serverConf._locations;
}

ft::ConfigServer::~ConfigServer()
{
	//Нужно очистить выделенную память для _location's
}

//getters
const std::string	&ft::ConfigServer::getPort() const {	return (_port); }
const std::string	&ft::ConfigServer::getHost() const {	return (_host); }

