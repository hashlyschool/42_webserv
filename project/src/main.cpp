#include "../inc/Server.hpp"

int	main(int argc, char **argv)
{
	std::string	path_config;
	if (argc == 1)
	{
		std::cout << "Using default config_path: \"../default_config\"" << std::endl;
		path_config = "../config";
	}
	else if (argc == 2)
		path_config = argv[1];
	else
		std::cout << "Using ./webserv [config_path]" << std::endl;
	Server x(path_config);
	// x.loop();
}
