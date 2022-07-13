#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>

namespace FT {

	class SimpleSocket {
		private:
			int socket_file_descriptor;
			struct sockaddr_in address;

		public:
			SimpleSocket(int domain, int service, int protocol, int port, u_long interface);
			virtual ~SimpleSocket();

			// Virtual function to connect to network
			virtual int connect_to_network(int socket_file_descriptor, struct sockaddr_in address) = 0;

			// Function to test sockets and connections
			void test_connection(int item_to_test);

			// Getters
			int get_socket_file_descriptor();
			struct sockaddr_in get_address();
	};

}
