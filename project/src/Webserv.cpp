#include "../inc/Webserv.hpp"

ft::Webserv::Webserv(std::string pathConf) : _parser(pathConf), _responder()
{
	int			temp_port;
	std::string	temp_host;

	FD_ZERO(&_mRead);
	FD_ZERO(&_mWrite);
	FD_SET(0, &_mRead);
	_num = 0;
	try
	{
		for (size_t i = 0; i < _parser.getConfigServers().size(); i++)
		{
			temp_host = _parser.getConfigServers().at(i).getHost();
			temp_port = _parser.getConfigServers().at(i).getPort();
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

void	ft::Webserv::printHelp() const
{
	std::cout <<	"------------------\n" << \
					"To turn off the web server, type: \'EXIT\' or \'Ctrl+D\'" << \
					"\n------------------\n";
}

void	ft::Webserv::processStdInput()
{
	std::string	inputStr;

	if (!std::getline(std::cin, inputStr))
		return ;
	if (inputStr.compare("EXIT") == 0)
		_num = 0;
	else
		printHelp();
}

void	ft::Webserv::createClientSocket(Socket *socket, int i)
{
	struct sockaddr		address;
	struct sockaddr_in&	addressIn = reinterpret_cast<struct sockaddr_in&>(address);
	socklen_t			addressLen = sizeof(address);

	memset(&address, 0, sizeof(address));
	addressIn.sin_family = AF_INET;
	addressIn.sin_port = socket->get_port();
	addressIn.sin_addr.s_addr = socket->get_host();

	int fd = accept(socket->get_socket_fd(), &address, &addressLen);
	if (fd < 0)
	{
		perror("accept");
		return ;
	}
	std::cout << "new accept fd = " << fd << std::endl;
	if (fd > _num)
		_num = fd;

	FD_SET(fd, &_mRead);
	// FD_SET(fd, &_tRead);
	_clientSocket.push_back(fd);
	_dataResr.dataFd.insert(std::make_pair(fd, new t_dataFd));
	_dataResr.dataFd[fd]->statusFd = ft::Nosession;
	_dataResr.dataFd[fd]->sendBodyByte = 0;
	_dataResr.dataFd[fd]->configServer = &(_parser.getConfigServers().at(i));
	_dataResr.dataFd[fd]->requestHead.clear();
	_dataResr.dataFd[fd]->requestBody.clear();
}

void	ft::Webserv::readFromClientSocket(int &fd)
{
	_responder.action(fd, _dataResr);
	if (_dataResr.dataFd[fd]->statusFd == ft::Send ||
	_dataResr.dataFd[fd]->statusFd == ft::Sendbody)
	{
		FD_CLR(fd, &_mRead);
		// FD_CLR(fd, &_tRead);
		FD_SET(fd, &_mWrite);
		// FD_SET(fd, &_tWrite);
	}
}

void	ft::Webserv::sendToClientSocket(int &fd)
{
	_responder.action(fd, _dataResr);
	if (_dataResr.dataFd[fd]->statusFd == ft::Closefd)
	{
		close(fd);
		_fdForDelete.push_back(fd);
		FD_CLR(fd, &_mWrite);
		delete _dataResr.dataFd[fd];
		_dataResr.dataFd.erase(fd);
	}
}

void	ft::Webserv::freeMemory()
{
	_dataResr.dataFd.size();
	for(std::map<int,t_dataFd *>::iterator iter = _dataResr.dataFd.begin(); iter != _dataResr.dataFd.end(); ++iter)
	{
		delete iter->second;
		// int fd =  iter->first;
	}
}

void	ft::Webserv::removeFdClientSocket()
{
	for (std::list<int>::iterator it = _fdForDelete.begin(); it != _fdForDelete.end(); ++it)
	{
		_clientSocket.remove(*it);
		if (*it == _num)
			_num--;
	}
	_fdForDelete.clear();
}

void	ft::Webserv::serverRun()
{
	while (_num && std::cin)
	{
		_tRead = _mRead;
		_tWrite = _mWrite;

		if (select(_num + 1, &_tRead, &_tWrite, 0, 0) <= 0)
			continue ;
		for (size_t i = 0; i < _sockets.size(); ++i)
		{
			if (FD_ISSET(_sockets.at(i)->get_socket_fd(), &_tRead))
				createClientSocket(_sockets.at(i), i);
		}
		for (std::list<int>::iterator it = _clientSocket.begin(); it != _clientSocket.end(); ++it)
		{
			if (FD_ISSET(*it, &_tRead))
				readFromClientSocket(*it);
			else if (FD_ISSET(*it, &_tWrite))
				sendToClientSocket(*it);
		}
		removeFdClientSocket();
		if (FD_ISSET(0, &_tRead))
			processStdInput();
	}
	freeMemory();
}
