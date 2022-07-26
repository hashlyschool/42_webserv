#include "../inc/Webserv.hpp"
// #include <stdio.h>
#include <signal.h>

void signal_handler(int signal)
{
	if (signal)
		signal = 0;
	std::cout <<	"------------------\n" << \
					"To turn off the web server, type: \'EXIT\' or \'Ctrl+D\'" << \
					"\n------------------\n";
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cout << "[ERROR] for start webserv using: ./webserv path_conf\n";
		return (1);
	}
	signal(SIGPIPE, SIG_IGN);
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, signal_handler);
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
