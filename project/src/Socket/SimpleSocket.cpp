 #include "../../inc/Socket/SimpleSocket.hpp"

// Constructor
FT::SimpleSocket::SimpleSocket(int domain, int service, int protocol, int port, u_long interface) {
	// Define address structure
	address.sin_family = domain;
	// Need to change byte order of port/interface
	address.sin_port   = htons(port);
	address.sin_addr.s_addr = htonl(interface);  // your ip add

	// Establish socket
	socket_file_descriptor = socket(domain, service, protocol);
	test_connection(socket_file_descriptor);
}

// Destructor close the socket
FT::SimpleSocket::~SimpleSocket() {
	if (socket_file_descriptor >= 0) {
		close(socket_file_descriptor);
	}
}

// Virtual function to test the connection
void FT::SimpleSocket::test_connection(int item_to_test) {
  // Confirm that the socket or connection has been established
  if (item_to_test < 0) {
    perror("Failed to connect");
    exit(EXIT_FAILURE);
  }
}

// Getter - return socket
int FT::SimpleSocket::get_socket_file_descriptor() {
  return socket_file_descriptor;
}

// Getter return the address structure
struct sockaddr_in FT::SimpleSocket::get_address() {
  return address;
}
