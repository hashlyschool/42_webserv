// #include "../inc/Socket/ListeningSocket.hpp"

// int main() {
// 	std::cout << "Starting..." << std::endl;

// 	std::cout << "Binding Socket..." << std::endl;
// 	FT::BindingSocket bs = FT::BindingSocket(AF_INET, SOCK_STREAM, 0, 8080, INADDR_ANY);

// 	std::cout << "Listening Socket..." << std::endl;
// 	FT::ListeningSocket ls = FT::ListeningSocket(AF_INET, SOCK_STREAM, 0, 3002, INADDR_ANY, 10);

// 	std::cout << "Success" << std::endl;
// }

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
