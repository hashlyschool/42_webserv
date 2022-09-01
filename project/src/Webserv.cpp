#include "../inc/Webserv.hpp"

ft::Webserv::Webserv(std::string pathConf) : _parser(pathConf), _responder()
{
	u_short			temp_port;
	in_addr_t		temp_host;

	FD_ZERO(&_mRead);
	FD_ZERO(&_mWrite);
	FD_SET(STDIN_FILENO, &_mRead);
	_num = 0;
	fillTimeout();
	for (size_t i = 0; i < _parser.getConfigServers().size(); ++i)
	{
		temp_host = _parser.getConfigServers().at(i).getHost();
		temp_port = _parser.getConfigServers().at(i).getPort();
		_sockets.push_back(new ft::Socket(temp_port, temp_host));
		FD_SET(_sockets.at(i)->get_socket_fd(), &_mRead);
		if (_sockets.at(i)->get_socket_fd() > _num)
			_num = _sockets.at(i)->get_socket_fd();
	}
}

// Destructor
ft::Webserv::~Webserv()
{
	for (size_t i = 0; i < _sockets.size(); i++)
		delete (_sockets.at(i));
	_dataResr.dataFd.size();
	for(std::map<int,t_dataFd *>::iterator iter = _dataResr.dataFd.begin(); iter != _dataResr.dataFd.end(); ++iter)
		delete iter->second;
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
		throw std::runtime_error("error accept");
	std::cout << "[INFO] new accept fd = " << fd << std::endl;
	FD_SET(fd, &_mRead);
	_clientSocket.push_back(fd);
	_num = ft::Utils::findMaxElem(_clientSocket) + 1;
	_dataResr.dataFd.insert(std::make_pair(fd, new t_dataFd));
	_dataResr.dataFd[fd]->statusFd = ft::Nosession;
	_dataResr.dataFd[fd]->sendBodyByte = 0;
	_dataResr.dataFd[fd]->configServer = &(_parser.getConfigServers().at(i));
	_dataResr.dataFd[fd]->requestHead.clear();
	_dataResr.dataFd[fd]->requestBody.clear();
	gettimeofday(&_dataResr.dataFd[fd]->timeLastAction, NULL);
}

void	ft::Webserv::readFromClientSocket(int &fd)
{
	_responder.action(fd, _dataResr);
	if (_dataResr.dataFd[fd]->statusFd == ft::Send ||
	_dataResr.dataFd[fd]->statusFd == ft::Sendbody)
	{
		FD_CLR(fd, &_mRead);
		FD_SET(fd, &_mWrite);
	}
}

void	ft::Webserv::sendToClientSocket(int &fd)
{
	_responder.action(fd, _dataResr);
	if (_dataResr.dataFd[fd]->statusFd == ft::Closefd)
	{
		_responder.action(fd, _dataResr);
		_fdForDelete.push_back(fd);
		FD_CLR(fd, &_mWrite);
	}
}

void	ft::Webserv::sendErrorToClientSocket(int &fd)
{
	if (!fd)
		return ;
	// // send code 500? and close connection
	// _dataResr.dataFd[fd]->httpRespone.sendErrr(500, "Max connection");
}

void	ft::Webserv::checkTimeConnection(int &fd)
{
	struct timeval time;

	gettimeofday(&time, NULL);
	//keep-alive
	if (time.tv_sec - _dataResr.dataFd[fd]->timeLastAction.tv_sec >= MAX_TIME_CONNECTION)
	{
		_dataResr.dataFd[fd]->statusFd = Closefd;
		_responder.action(fd, _dataResr);
		_fdForDelete.push_back(fd);
		if (FD_ISSET(fd, &_mWrite))
			FD_CLR(fd, &_mWrite);
		if (FD_ISSET(fd, &_mRead))
			FD_CLR(fd, &_mRead);
	}
}

void	ft::Webserv::removeFdClientSocket()
{
	_fdForDelete.sort();
	_fdForDelete.unique();
	for (std::list<int>::iterator it = _fdForDelete.begin(); it != _fdForDelete.end(); ++it)
	{
		_clientSocket.remove(*it);
		if (*it == _num)
			_num--;
	}

	_fdForDelete.clear();
}

void	ft::Webserv::fillTimeout()
{
	_timeout.tv_sec = 1; //maybe MaxConnection
	_timeout.tv_usec = 0;
}

void	ft::Webserv::serverRun()
{
	while (_num && std::cin)
	{
		_tRead = _mRead;
		_tWrite = _mWrite;

		if (select(_num + 1, &_tRead, &_tWrite, 0, &_timeout) <= 0)
		{
			fillTimeout();
			if (_clientSocket.size() == 0)
				continue ;
		}
		fillTimeout();
		for (size_t i = 0; i < _sockets.size(); ++i)
		{
			if (FD_ISSET(_sockets.at(i)->get_socket_fd(), &_tRead))
			{
				createClientSocket(_sockets.at(i), i);
				if (_clientSocket.size() > MAX_CONNECTION)
					sendErrorToClientSocket(_clientSocket.back());
			}
		}
		for (std::list<int>::iterator it = _clientSocket.begin(); it != _clientSocket.end(); ++it)
		{
			checkTimeConnection(*it);
			if (FD_ISSET(*it, &_tRead))
				readFromClientSocket(*it);
			else if (FD_ISSET(*it, &_tWrite))
				sendToClientSocket(*it);
		}
		removeFdClientSocket();
		if (FD_ISSET(0, &_tRead))
			processStdInput();
	}
}

// Exceptions
const char *ft::Webserv::Exception::what() const throw()
{
	return ("Webserv exception");
}
