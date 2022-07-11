#include <string>
#include <iostream>
#include <./Logger.hpp>

class Server
{
	public:
		Server();
		Server(std::string path_config);
		~Server();
	private:
		std::string	_path_config;
};


