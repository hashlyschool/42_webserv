#pragma once

#include <arpa/inet.h>
#include <netdb.h>
#include <string>
#include <cstdlib>
#include "Location.hpp"
#include "ALocation.hpp"
#include <vector>

namespace ft
{
	struct Location;

	class ConfigServer : public ft::ALocation
	{
	private:
		// tcp communication configuration
		u_short _port;
		in_addr_t _host;
		std::string _serverName;
		int _maxBodySize;

		// // data parsed from web server conf file
		std::vector<Location> _locations;

	public:
		ConfigServer();
		ConfigServer &operator=(const ConfigServer &other);
		virtual ~ConfigServer();

		void setPort(const int &port);
		const u_short &getPort() const;

		void setHost(const std::string &host);
		const in_addr_t &getHost() const;

		void setServerName(const std::string &serverName);
		const std::string &getServerName() const;

		void setMaxBodySize(const int &maxBodySize);
		const int &getMaxBodySize() const;

		void setLocations(const std::vector<Location> &locs);
		std::vector<Location> &getLocations();
		Location &getLocation(std::string &url);
	};

}
