#include "../inc/ConfigServer.hpp"

ft::ConfigServer::ConfigServer(std::map<std::string, std::string> param, std::map<int, std::string> err_page, std::vector<Location> loc) : server_directives(param), server_error_page_location(err_page), server_locations(loc)
{
	// The standard form is "listen  127.0.0.1:8080"
	// We assume that our servers listen on all ip address, i.e., "8080" means "0.0.0.0:8080"
	std::string listen_param = param.find("listen")->second;
	size_t seperator_pos = listen_param.find(':');
	std::string ipPart = listen_param.substr(0, seperator_pos);
	std::string portPart = listen_param.substr(seperator_pos + 1);
	server_host = (seperator_pos != std::string::npos) ? ipPart : "0.0.0.0";
	server_port = (seperator_pos != std::string::npos) ? atoi(portPart.c_str()) : atoi(listen_param.c_str());
}

ft::ConfigServer::~ConfigServer()
{
	//Нужно очистить выделенную память для _location's
}

//getters
const int	&ft::ConfigServer::getPort() const {	return (server_port); }
const std::string	&ft::ConfigServer::getHost() const {	return (server_host); }

