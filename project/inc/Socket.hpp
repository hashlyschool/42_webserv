#pragma once

#include <cstdlib>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

namespace ft
{

	class Socket
	{
		private:
			int					_socket_fd;
			struct sockaddr_in	_address;

		public:
			Socket(int port, std::string host, int backlog = 10);
			virtual				~Socket();

			// Getters
			int					get_socket_fd();
			in_addr_t			get_host();
			in_port_t			get_port();
	};

}
