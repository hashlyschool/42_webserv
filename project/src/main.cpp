#include <iostream>

#include "../inc/Server/SimpleHTTPServer.hpp"

int main()
{
	// insert code here...
	std::cout << "Hello from mainline\n";

	FT::SimpleHTTPServer *simpleHTTPServer = new FT::SimpleHTTPServer();

	std::cout << "Exiting mainline\n";

	delete simpleHTTPServer;
	return 0;
}
