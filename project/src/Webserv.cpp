#include "../inc/Webserv.hpp"

ft::Webserv::Webserv(std::string pathConf) : Parser(pathConf), Responder()
{
	int			temp_port;
	std::string	temp_host;

	FD_ZERO(&_mRead);
	FD_ZERO(&_mWrite);
	FD_SET(0, &_mRead);
	_num = 0;
	try
	{
		for (int i = 0; i < getNumServers(); i++)
		{
			temp_host = getConfigServer(i)->getHost();
			temp_port = std::atoi(getConfigServer(i)->getPort().c_str());
			_sockets.push_back(new ft::Socket(temp_port, temp_host, 10));
			FD_SET(_sockets.at(i)->get_socket_fd(), &_mRead);
			if (_sockets.at(i)->get_socket_fd() > _num)
				_num = _sockets.at(i)->get_socket_fd();
		}
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}
}

// Destructor
ft::Webserv::~Webserv()
{
	for (size_t i = 0; i < _sockets.size(); i++)
		delete (_sockets.at(i));
}

void	ft::Webserv::createClientSocket(Socket *socket)
{
	struct sockaddr		address;
	struct sockaddr_in&	addressIn = reinterpret_cast<struct sockaddr_in&>(address);
	socklen_t			addressLen;

	addressIn.sin_port = socket->get_port();
	addressIn.sin_addr.s_addr = socket->get_host();

	int fd = accept(socket->get_socket_fd(), &address, &addressLen);

	if (fd < 0) {
		return ;
	}
	if (fd > _num) {
		_num = fd;
	}
	FD_SET(fd, &_mRead);
	_clientSocket.push_back(fd);
}

void	ft::Webserv::printHelp() const
{
	std::cout <<	"------------------\n" << \
					"For over working server print EXIT" << \
					"\n------------------\n";
}

void	ft::Webserv::processStdInput()
{
	std::string	inputStr;

	if (!std::getline(std::cin, inputStr))
	{
		return ;
	}
	if (inputStr.compare("EXIT") == 0)
		_num = 0;
	else
		printHelp();
}

void	ft::Webserv::serverRun()
{
	char	buf[2048];
	fd_set	readFd;
	fd_set	writeFd;

	FD_SET(0, &_mRead);
	while (_num)
	{
		readFd = _mRead;
		writeFd = _mWrite;

		if (select(_num, &readFd, &writeFd, 0, 0) <= 0)
			continue ;

		if (FD_ISSET(0, &readFd))
		{
			processStdInput();
			continue;
		}

		for (size_t i = 0; i < _sockets.size(); ++i)
		{
			if (FD_ISSET(_sockets.at(i)->get_socket_fd(), &readFd))
				createClientSocket(_sockets.at(i));
		}

		for (std::list<int>::iterator it = _clientSocket.begin(); it != _clientSocket.end(); ++it)
		{
			if (FD_ISSET(*it, &readFd))
			{
				recv(*it, buf, 2048, 0);
				std::cout << "----------------\n";
				std::cout << buf;
				std::cout << "----------------\n";
				FD_CLR(*it, &_mRead);
				FD_SET(*it, &_mWrite);
			}
		}
		for (std::list<int>::iterator it = _clientSocket.begin(); it != _clientSocket.end(); ++it)
		{
			if (FD_ISSET(*it, &writeFd))
			{
				action(*it);
				FD_CLR(*it, &_mWrite);
			}
		}
	}
}
