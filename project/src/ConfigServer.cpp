#include "../inc/ConfigServer.hpp"

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

void ft::ConfigServer::setLocations(const std::vector<Location> &locs)
{
	_locations = locs;
}

std::vector<ft::Location> &ft::ConfigServer::getLocations()
{
	return _locations;
}

ft::Location *ft::ConfigServer::getLocation(std::string &url)
{
	std::vector<Location>::iterator v_loc = getLocations().begin();
	while (v_loc != getLocations().end())
	{
		if ((url.find(v_loc->getUrl()) == 0) || (url + "/").find(v_loc->getUrl()) == 0)
			break;
		v_loc++;
	}
	if (v_loc != getLocations().end())
		return &(*v_loc);
	return NULL;
}
