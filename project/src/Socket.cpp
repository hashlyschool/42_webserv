 #include "../inc/Socket.hpp"

// Constructor
ft::Socket::Socket(u_short port, in_addr_t host, int backlog)
{
	_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	int opt = 1;
	if (setsockopt(_socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) < 0)
		throw std::runtime_error("error setsockopt");
	if (_socket_fd < 0)
		throw std::runtime_error("invalid socket");
	memset(&_address, 0, sizeof(_address));
	_address.sin_family = AF_INET;
	_address.sin_port = port;
	_address.sin_addr.s_addr = host;

	if (bind(_socket_fd, (struct sockaddr *)&_address, sizeof(_address)) < 0)
		throw std::runtime_error("invalid bind");
	if (listen(_socket_fd, backlog) < 0)
		throw std::runtime_error("invalid listen");
}

// Destructor close the socket
ft::Socket::~Socket() {
	if (_socket_fd >= 0) {
		close(_socket_fd);
		_socket_fd = -1;
	}
}

// Getters
int			ft::Socket::get_socket_fd() {	return _socket_fd;}
in_addr_t	ft::Socket::get_host() {	return _address.sin_addr.s_addr;}
in_port_t	ft::Socket::get_port() {	return _address.sin_port;}

