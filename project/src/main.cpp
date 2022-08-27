#include "../inc/Webserv.hpp"

int	main(int argc, char **argv)
{
	try
	{
		ft::Utils::checkArg(argc, argv);
		ft::Utils::setSignal();
		ft::Webserv WebServer(argv[1]);
		WebServer.serverRun();
	}
	catch(const std::exception& e)
	{
		std::cerr << "Call exception:\n" << e.what() << '\n';
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
