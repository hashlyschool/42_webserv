
#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "./ConfigServer.hpp"

namespace ft{

	class Parser {
		private:
		//	struct servers {
		//		
		//	};
			std::vector<ConfigServer*>	_configServers;
			t_serverConf				parseOneServer();
			
			Parser();
			std::string	Split(std::string &line, std::string delimiter);
			void checkBrackets();
			std::string _config;

		public:
			Parser(std::string conf);
			~Parser();
			
			int					getNumServers() const;
			const ConfigServer*	getConfigServer(int index) const;
			void Parse();
	};
}
