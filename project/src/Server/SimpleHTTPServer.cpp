#include "../../inc/Server/SimpleHTTPServer.hpp"

// Constructor, call constructor of simple server and pass values
FT::SimpleHTTPServer::SimpleHTTPServer() : SimpleServer(AF_INET, SOCK_STREAM, 0, 8080, INADDR_ANY, 10) {
	launch();
}

// Accept connection from client and read request
void FT::SimpleHTTPServer::accepter() {

	// Define some local vars for the accept call
	struct sockaddr_in address = get_socket()->get_address();
	int addressLen = sizeof(address);

	socketForClient = accept(get_socket()->get_socket_file_descriptor(),(struct sockaddr *)&address,(socklen_t *)&addressLen);

	// Make sure got a valid connection
	get_socket()->test_connection(socketForClient);

	// Read from the socket
	read(socketForClient, buffer, BUFFSIZE);
}

// Handle the request
void FT::SimpleHTTPServer::handler() {
	std::cout << buffer << std::endl;
}

// Generate response
void FT::SimpleHTTPServer::responder() {
	std::string	response("HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!");
	write(socketForClient,response.c_str(), response.length());
	close(socketForClient);
}
