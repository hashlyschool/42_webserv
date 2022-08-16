#pragma once

#include <arpa/inet.h>
#include <netdb.h>
#include <string>
#include <cstdlib>
#include "./Parser.hpp"

namespace ft
{
	struct Location;

	class ConfigServer
	{
	private:
		// tcp communication configuration
		std::string server_host;
		int server_port;
		int server_socket;

		// data parsed from web server conf file
		std::map<std::string, std::string> server_directives;
		std::map<int, std::string> server_error_page_location;
		std::vector<Location> server_locations;

	public:
		ConfigServer(std::map<std::string, std::string> param, std::map<int, std::string> err_page, std::vector<Location> loc);
		~ConfigServer();

		const int &getPort() const;
		const std::string &getHost() const;
	};

}
