#include "../../inc/Server/HTTPServer.hpp"

// Constructor, call constructor of simple server and pass values
FT::HTTPServer::HTTPServer() : SimpleServer(AF_INET, SOCK_STREAM, 0, 8080, INADDR_ANY, 10) {
	try {
		registerSignal();
		std::cout << "Using default path_config: ./default_config" << std::endl;
		parseConfig();
		launch();
	}
	catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}
}

void FT::HTTPServer::parseConfig() {
/*
    //example
	{
		Utils::tokenizeFileStream(config, _tok_list);
    std::list<std::string>::iterator end = _tok_list.end();
    std::list<std::string>::iterator it  = _tok_list.begin();
    std::string                      mime_conf_path;
    for (; it != end; ++it) // loop for servers
    {
        if (*it != "server" || *(++it) != "{")
            throw Server::ServerException("Error while parsing config file");
        VirtualServer serv;
        ++it;
        std::list<std::string>::iterator check;
        while (it != end && *it != "}") //loop inside server block
        {
            if (it != end && *it == "listen") {
                serv.setHost(it, end);
                continue;
            }
            if (it != end && *it == "port") {
                serv.setPort(it, end);
                continue;
            }
            if (it != end && *it == "client_max_body_size") {
                serv.setBodySize(it, end);
                continue;
            }
            if (it != end && *it == "error_page") {
                serv.setErrorPage(it, end);
                continue;
            }
            if (it != end && *it == "server_name") {
                serv.setServerName(it, end);
                continue;
            }
            if (it != end && *it == "location") {
                serv.setLocation(it, end);
                continue;
            }
            if (it != end && *it == "mime_conf_path") {
                Utils::skipTokens(it, end, 1);
                mime_conf_path = *it;
            }
            ++it;
        }
        if (mime_conf_path.empty())
            throw Server::ServerException("No mime.conf path");
        MimeType(mime_conf_path.c_str());
        validate(serv);
        apply(serv);
    }
	}
*/
	std::cout << "Parse config: '" << "default_config" << std::endl;
}


// Accept connection from client and read request
void FT::HTTPServer::accepter() {

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
void FT::HTTPServer::handler() {
	//need processing signals
	std::cout << buffer << std::endl;
}

// Generate response
void FT::HTTPServer::responder()
{
	std::string	response("HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!");
	write(socketForClient,response.c_str(), response.length());
	close(socketForClient);
}
