#include "../../inc/Server/SimpleServer.hpp"

// Constructor
FT::SimpleServer::SimpleServer(int domain, int service, int protocol, int port, u_long interface, int backlog)
{

	keeprunning = true;

	listeningSocket = new ListeningSocket(domain, service, protocol, port, interface, backlog);
}

// Destructor
FT::SimpleServer::~SimpleServer() {
	delete listeningSocket;
}

void signal_handler(int signal) {
	std::cout << "stopping on signal " << signal << std::endl;
	// std::cout << "Using command \'EXIT or HELP\'" << std::endl;
	exit(signal);
}

void FT::SimpleServer::registerSignal() {
	//Сигналы нужно игнорировать
	//Нужно читать с консоли и при EXIT закрывать сервер
	//но пока сигналы обрабатываются
	signal(SIGPIPE, SIG_IGN);
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, signal_handler);
}

// Setter to be able to control keeprunning flag
void FT::SimpleServer::setKeeprunning(bool keeprunning) {
	this->keeprunning = keeprunning;
}

FT::ListeningSocket * FT::SimpleServer::get_socket() {
	return	listeningSocket;
}

// Launch server
void FT::SimpleServer::launch() {
	while(keeprunning) {
		accepter();
		handler();
		responder();
	}
}
