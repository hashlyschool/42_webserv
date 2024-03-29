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
	_dataResr.size();
	for(std::map<int,DataFd *>::iterator iter = _dataResr.begin(); iter != _dataResr.end(); ++iter)
		delete iter->second;
}

void	ft::Webserv::printHelp() const
{
	std::cout <<	"------------------\n" << \
					"To turn off the web server, type: \'EXIT\' or \'Ctrl+D\'" << \
					"\n------------------\n";
}

void	ft::Webserv::fdClr(int &fd, fd_set &set)
{
	if (FD_ISSET(fd, &set))
		FD_CLR(fd, &set);
}

void	ft::Webserv::fdSet(int &fd, fd_set &set)
{
	if (!FD_ISSET(fd, &set))
		FD_SET(fd, &set);
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
	fdSet(fd, _mRead);
	_clientSocket.push_back(fd);
	setNum();
	_dataResr.insert(std::make_pair(fd, new DataFd(fd)));
	_dataResr[fd]->configServer = &(_parser.getConfigServers().at(i));
}

void	ft::Webserv::readFromClientSocket(int &fd)
{
	if (_dataResr.find(fd) == _dataResr.end())
		return ;
	_responder.action(fd, _dataResr);
	if (_dataResr[fd]->statusFd == ft::SendHead ||
	_dataResr[fd]->statusFd == ft::Sendbody ||
	_dataResr[fd]->statusFd == ft::Execute ||
	_dataResr[fd]->statusFd == ft::CGI)
	{
		fdClr(fd, _mRead);
		fdSet(fd, _mWrite);
	}
}

void	ft::Webserv::sendToClientSocket(int &fd)
{
	if (_dataResr.find(fd) == _dataResr.end())
		return ;
	_responder.action(fd, _dataResr);
	if (_dataResr[fd]->statusFd == ft::Nosession)
	{
		fdClr(fd, _mWrite);
		fdSet(fd, _mRead);
	}
	else if (_dataResr[fd]->statusFd == ft::Closefd)
	{
		_responder.action(fd, _dataResr);
		_fdForDelete.push_back(fd);
		fdClr(fd, _mWrite);
		fdClr(fd, _mRead);
	}
}

void	ft::Webserv::sendErrorToClientSocket(int &fd, HTTPStatus status)
{
	//set status
	if (_dataResr[fd]->statusFd == ft::Readbody ||
		_dataResr[fd]->statusFd == ft::Readhead)
	{
		_dataResr[fd]->code = status;
		//send head
		_dataResr[fd]->statusFd = ft::SendHead;
		_responder.action(fd, _dataResr);
	}
	//close fd
	_dataResr[fd]->statusFd = Closefd;
	_responder.action(fd, _dataResr);
	//remove fd
	_fdForDelete.push_back(fd);
	fdClr(fd, _mWrite);
	fdClr(fd, _mRead);
}

void	ft::Webserv::checkTimeConnection(int &fd)
{
	struct timeval time;

	if (gettimeofday(&time, NULL) < 0 || _dataResr.find(fd) == _dataResr.end())
		return ;
	if (time.tv_sec - _dataResr[fd]->getTimeLastAct().tv_sec >= MAX_TIME_CONNECTION)
		sendErrorToClientSocket(fd, HTTP_REQUEST_TIMEOUT);
}

void	ft::Webserv::setNum()
{
	if (_clientSocket.size() != 0)
		_num = ft::Utils::findMaxElem(_clientSocket) + 1;
	else
	{
		_num = _sockets.at(0)->get_socket_fd() + 1;
		for (size_t i = 1; i < _sockets.size(); ++i)
		{
			if (_sockets.at(i)->get_socket_fd() > _num)
				_num = _sockets.at(1)->get_socket_fd() + 1;
		}
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
					sendErrorToClientSocket(_clientSocket.back(), HTTP_SERVICE_UNAVAILABLE);
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
