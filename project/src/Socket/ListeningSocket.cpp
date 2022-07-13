#include "../../inc/Socket/ListeningSocket.hpp"

FT::ListeningSocket::ListeningSocket(int domain, int service, int protocol, int port, u_long interface, int backlog) : BindingSocket(domain, service, protocol, port, interface) {
	this->backlog = backlog;

	// Call start_listening and check return code
	int listen_rc = start_listening();
	test_connection(listen_rc);
}

int FT::ListeningSocket::start_listening() {
	return listen(get_socket_file_descriptor(), backlog);
}
