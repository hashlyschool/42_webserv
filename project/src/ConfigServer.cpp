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
	std::vector<Location>::iterator loc_itr = getLocations().begin();
	std::vector<ft::Location *> v_loc;

	while (loc_itr != getLocations().end())
	{
		if ((url.find(loc_itr->getUrl()) == 0) || (url + "/").find(loc_itr->getUrl()) == 0)
			v_loc.push_back(&(*loc_itr));
		loc_itr++;
	}
	if (v_loc.size() == 1)
		return (v_loc[1]);
	if (v_loc.size() > 1)
	{
		sort(v_loc.begin(), v_loc.end(), comp_loc_url);
		return (v_loc[0]);
	}
	return NULL;
}

bool ft::comp_loc_url(ft::Location *loc1, ft::Location *loc2)
{
	return (loc1->getUrl() < loc2->getUrl());
}
