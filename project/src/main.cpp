#include "../inc/Webserv.hpp"
#include <signal.h>

void	signal_handler(int signal)
{
	if (signal)
		signal = 0;
	std::cout <<	"--------[INFO]--------\n" << \
					"To turn off the web server, type: \'EXIT\' or \'Ctrl+D\'\n" << \
					"--------[INFO]--------\n";
}

void	check_arg_set_signal(int argc)
{
	if (argc != 2)
	{
		std::cout << "[ERROR] for start webserv using: ./webserv path_conf\n";
		exit(1);
	}
	signal(SIGPIPE, SIG_IGN);
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, signal_handler);
}

int	main(int argc, char **argv)
{
	check_arg_set_signal(argc);
	try
	{
		ft::Webserv WebServer(argv[1]);
		WebServer.serverRun();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	return (0);
}
