#pragma once

#include "./Location.hpp"
#include <vector>

namespace ft
{
	typedef struct	s_serverConf
	{
		std::string				_port;
		std::string				_host;
		std::string				_serverName;
		int						_maxBodySize;
		std::vector<Location*>	_locations;
	}				t_serverConf;

	class ConfigServer : public ALocation
	{
		private:
			std::string				_port;
			std::string				_host;
			std::string				_serverName;
			int						_maxBodySize;
			std::vector<Location*>	_locations;
		public:
			ConfigServer(t_serverConf serverConf);
			virtual ~ConfigServer();

			//Getters
			const std::string &getPort() const;
			const std::string &getHost() const;

	};

}
