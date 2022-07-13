#include "../../inc/Server/SimpleServer.hpp"

// Constructor
FT::SimpleServer::SimpleServer(int domain, int service, int protocol, int port, u_long interface, int backlog) {

  keeprunning = true;

  listeningSocket = new ListeningSocket(domain, service, protocol, port, interface, backlog);
}

// Destructor
FT::SimpleServer::~SimpleServer() {
	delete listeningSocket;
}

// Setter to be able to control keeprunning flag
void FT::SimpleServer::setKeeprunning(bool keeprunning) {
  this->keeprunning = keeprunning;
}

FT::ListeningSocket * FT::SimpleServer::get_socket() {
  return listeningSocket;
}

// Launch server
void FT::SimpleServer::launch() {
  while(keeprunning) {
    accepter();
    handler();
    responder();
  }
}
