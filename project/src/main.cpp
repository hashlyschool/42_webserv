#include <iostream>

#include "../inc/Server/HTTPServer.hpp"

int main()
{
	// insert code here...
	std::cout << "Hello from mainline\n";

	FT::HTTPServer *HTTPServer = new FT::HTTPServer();

	std::cout << "Exiting mainline\n";

	delete HTTPServer;
	return 0;
}
