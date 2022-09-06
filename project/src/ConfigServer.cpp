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

const std::vector<ft::Location> &ft::ConfigServer::getLocations() const
{
	return _locations;
}

const ft::ALocation *ft::ConfigServer::getLocation(std::string &url) const
{
	std::vector<const ft::Location *> v_loc;

	for (size_t i = 0; i < getLocations().size(); i++)
	{
		const ft::Location &current = getLocations().at(i);
		if ((url.find(current.getUrl()) == 0) || (url + "/").find(current.getUrl()) == 0)
			v_loc.push_back(&current);
	}
	if (v_loc.size() == 1)
		return (v_loc[0]);
	if (v_loc.size() > 1)
	{
		sort(v_loc.begin(), v_loc.end(), comp_loc_url);
		return (v_loc[0]);
	}
	return this;
}

bool ft::comp_loc_url(const ft::Location *loc1, const ft::Location *loc2)
{
	return (loc1->getUrl() > loc2->getUrl());
}

std::string ft::ConfigServer::getFilename(std::string _url, const ft::ALocation &loc) const
{
	std::string filename;

	if (_url == "/")
	{
		if (getIndex() != "")
			filename = "./www/" + getServerName() + "/" + this->getRoot() + "/" + getIndex();
		else if (getAutoIndex())
		{
			filename = "./www/" + getServerName() + "/" + this->getRoot() + "/";
		}
		else
			filename = "./www/" + getServerName() + "/" + this->getRoot() + _url; // ??
	}
	else
	{
		if (loc.getRoot() != "")
		{
			filename = "./www/" + getServerName() + "/" + this->getRoot() + loc.getRoot();
			if (loc.getIndex() != "")
			{
				if ((_url == loc.getUrl()) || ((_url + "/") == loc.getUrl()))
				{
					filename = "./www/" + getServerName() + "/" + this->getRoot() + loc.getUrl() + loc.getIndex();
				}
				else
					filename = "./www/" + getServerName() + "/" + this->getRoot() + _url; // ?
			}
			else
			{
				if (this->getAutoIndex())
				{
					filename = "./www/" + getServerName() + "/" + this->getRoot() + _url + "/";
				}
			}
		}
		else // Location without root
		{
			if (loc.getIndex() != "") // Index is found
			{
				if ((_url == loc.getUrl()) || ((_url + "/") == loc.getUrl()))
				{
					filename = "./www/" + getServerName() + "/" + this->getRoot() + loc.getUrl() + loc.getIndex();
				}
				else
					filename = "./www/" + getServerName() + "/" + this->getRoot() + _url; // ?
			}
			else // Location without index page
			{
				if (this->getAutoIndex())
				{
					filename = "./www/" + getServerName() + "/" + this->getRoot() + _url + "/";
				}
			}
		}
	}
	return filename;
}
