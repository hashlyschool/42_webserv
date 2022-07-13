#pragma once

#include <stdio.h>
#include "./SimpleSocket.hpp"

namespace FT {

	// Class for binding socket, this is assigning an address (and port (for ip
	//   networks)) to a socket.  Some call it 'naming' a socket
	class BindingSocket: public SimpleSocket {
		public:
			// Constructor
			BindingSocket(int domain, int service, int protocol, int port, u_long interface);

			// Funtion to bind (connect to network)
			int connect_to_network(int socket_fd, struct sockaddr_in address);
	};
}
