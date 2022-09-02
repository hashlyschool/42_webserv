#include "../inc/Responder.hpp"

ft::Responder::Responder() {}

ft::Responder::~Responder() {}

void	ft::Responder::_makeSession(int &fd, t_dataResp &data)
{
	std::string	input;
	std::string inputHeader;
	size_t		startBody;
	data.dataFd[fd]->httpRequest = HttpRequest();
	HttpRequest &curRequest = data.dataFd[fd]->httpRequest;

	input = ft::Utils::readFromSocket(fd, BUF_SIZE);
	startBody = ft::Utils::getdelim(input, inputHeader, DELIMITER);
	curRequest.appendHead(inputHeader);
	if (startBody != std::string::npos)
	{
		curRequest.parseHeader();
		curRequest.readBody(input.substr(startBody));
		_setStatusRequest(data.dataFd[fd]);
	}
	// we could be really lucky to get delimiter in different portions of data
	else if ((startBody = ft::Utils::getdelim(curRequest.getRequestStr(), inputHeader, DELIMITER)) != std::string::npos)
	{
		// trim request head so that part of the body wouldn't be there
		// the remainder append to the body => it already contains the body from input; its fine

		std::string body = curRequest.getRequestStr().substr(startBody);
		curRequest.setRequestStr(inputHeader);
		curRequest.parseHeader();
		curRequest.readBody(body);
		_setStatusRequest(data.dataFd[fd]);
	}
}

void	ft::Responder::_readBody(int &fd, t_dataResp &data)
{
	std::string input = ft::Utils::readFromSocket(fd, BUF_SIZE);

	data.dataFd[fd]->httpRequest.readBody(input);
	//set status
	_setStatusRequest(data.dataFd[fd]);
}

void	ft::Responder::_send(int &fd, t_dataResp &data)
{
	//create response head
	// std::cout << "body = " << data.dataFd[fd]->httpRequest.getBody() << std::endl;
	// std::cout << "\nREQUEST\n\n" << data.dataFd[fd]->httpRequest.getRequestStr() << "\n\n";
	// std::cout << "\nURL\n\n" << data.dataFd[fd]->httpRequest.getURL() << "\n\n";
	data.dataFd[fd]->responseHead = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\n";
	//create response body
	data.dataFd[fd]->responseBody = "Hello world!";
	//send Head
	std::string &response = data.dataFd[fd]->responseHead;
	send(fd, response.c_str(), response.length(), 0);
	//set status
	if (data.dataFd[fd]->responseBody.length() == 0)
	{
		if (data.dataFd[fd]->httpRequest.getConnectionClosed())
			data.dataFd[fd]->statusFd = ft::Closefd;
		else
			data.dataFd[fd]->statusFd = ft::Nosession;
	}
	else
		data.dataFd[fd]->statusFd = ft::Sendbody;
}

void	ft::Responder::_sendBody(int &fd, t_dataResp &data)
{
	size_t		&sendByteNow = data.dataFd[fd]->sendBodyByte;

	//send Body
	std::string	response = data.dataFd[fd]->responseBody.substr(sendByteNow, sendByteNow + BUF_SIZE);
	send(fd, response.c_str(), response.length(), 0);
	//trim body
	sendByteNow += response.length();
	if (data.dataFd[fd]->responseBody.length() == sendByteNow)
	{
		if (data.dataFd[fd]->httpRequest.getConnectionClosed())
			data.dataFd[fd]->statusFd = ft::Closefd;
		else
			data.dataFd[fd]->statusFd = ft::Nosession;
	}
}

void	ft::Responder::_cgi(int &fd, t_dataResp &data)
{
	ft::Cgi	&cgi = data.dataFd[fd]->cgi;
	pid_t	pid;

	cgi.parseQueryString();
	cgi.preparseExecveData();
	pid = fork();
	if (fork() < 0)
		cgi.error();
	if (pid == 0)
		cgi.childProcess();
	else
		cgi.ParentProcess(pid);
	data.dataFd[fd]->responseHead = cgi.getResponseHead();
	// data.dataFd[fd]->responseBody = cgi.getResponseBody();
	data.dataFd[fd]->statusFd = ft::Send;

	//Попробовать подождать процесс дочерний
	//
	//createHead
	// data.dataFd[fd]->responseHead = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 22\n\n";
	//createBody
	// char	buf[2048];
	// bzero(buf, 2048);
	// read()
	// data.dataFd[fd]->responseBody = read
	// if (connection == keepAlive && time < MaxTime && Qrequests < max)
	// 	data.dataFd[fd]->statusFd = ft::Nosession;
	// else //close
	// 	data.dataFd[fd]->statusFd = ft::Closefd;
}

void	ft::Responder::_closeFd(int &fd, t_dataResp &data)
{
	std::cout << "[INFO] Close fd = " << fd << "\n";
	close(fd);
	delete data.dataFd[fd];
	data.dataFd.erase(fd);
	//if connection close or time -> makesession
	//else close
}

void	ft::Responder::_autoIndex(int &fd, t_dataResp &data)
{
	if (fd || data.dataFd[fd]->statusFd)
		fd = fd;
}

void	ft::Responder::action(int &fd, t_dataResp &data)
{
	int	status = data.dataFd[fd]->statusFd;

	gettimeofday(&data.dataFd[fd]->timeLastAction, NULL);
	switch (status)
	{
		case ft::Nosession:
			_makeSession(fd, data);
			break;
		case ft::Readbody:
			_readBody(fd, data);
			break;
		case ft::Send:
			_send(fd, data);
			break;
		case ft::Sendbody:
			_sendBody(fd, data);
			break;
		case ft::CGI:
			_cgi(fd, data);
			break;
		case ft::Closefd:
			_closeFd(fd, data);
			break;
		case ft::AutoIndex:
			_autoIndex(fd, data);
			break;
		default:
			break;
	}
}

void ft::Responder::_setStatusRequest(t_dataFd *data)
{
	if (data->httpRequest.bodyIsRead())
	{
		// if (isCGI())
		// 	data->statusFd = ft::CGI;
		// else
			data->statusFd = ft::Send;
	}
	else
		data->statusFd = ft::Readbody;
}
