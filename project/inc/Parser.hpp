#pragma once

#include "./ConfigServer.hpp"


namespace ft
{

	class Parser
	{
		private:
			std::vector<ConfigServer*>	_configServers;

			t_serverConf				parseOneServer();
			void						parseConfig(std::string pathConf);
		public:
			Parser(std::string pathConf);
			virtual ~Parser();

			//Getters
			int					getNumServers() const;
			const ConfigServer*	getConfigServer(int index) const;

	};

}
