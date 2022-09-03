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
		if (curRequest.parseHeader() < 0)
			data.dataFd[fd]->code = HTTP_BAD_REQUEST;
		if (!curRequest.getConnectionClosed())
			curRequest.readBody(input.substr(startBody));
		_setStatusRequest(data.dataFd[fd]);
	}
	// we could be really lucky to get delimiter in different portions of data
	else if ((startBody = ft::Utils::getdelim(curRequest.getRequestStr(), inputHeader, DELIMITER)) != std::string::npos)
	{
		// OR WE COULD DO ERROR 400 AND WHATEVER
		// trim request head so that part of the body wouldn't be there
		// the remainder append to the body => it already contains the body from input; its fine

		std::string body = curRequest.getRequestStr().substr(startBody);
		curRequest.setRequestStr(inputHeader);
		if (curRequest.parseHeader() < 0)
			data.dataFd[fd]->code = HTTP_BAD_REQUEST;
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

void ft::Responder::_execute(int &fd, t_dataResp &data)
{
	//create response head
	// std::cout << "body = " << data.dataFd[fd]->httpRequest.getBody() << std::endl;
	// std::cout << "\nREQUEST\n\n" << data.dataFd[fd]->httpRequest.getRequestStr() << "\n\n";
	// std::cout << "\nURL\n\n" << data.dataFd[fd]->httpRequest.getURL() << "\n\n";
	std::string method = data.dataFd[fd]->httpRequest.getMethod();
	if (method == "GET")
		_get(data.dataFd[fd]);
	else if (method == "POST")
		_post(data.dataFd[fd]);
	else
		_delete(data.dataFd[fd]);
	data.dataFd[fd]->statusFd = ft::SendHead;
}

void	ft::Responder::_sendHead(int &fd, t_dataResp &data)
{
	int	status;

	//create response head
	HttpResponse response = HttpResponse(*data.dataFd[fd]);
	data.dataFd[fd]->httpResponse = response;

	response.setBodyUrl(data.dataFd[fd]->finalUrl);
	// std::cout << "body = " << data.dataFd[fd]->httpRequest.getBody() << std::endl;
	std::string head = response.getResponseHead();
	//create response body
	// data.dataFd[fd]->responseBody = "Hello world!";
	// send Head
	// std::string &response = data.dataFd[fd]->responseHead;
	status = send(fd, head.c_str(), head.length(), 0);
	std::cout << "SendHead status = " << status << std::endl;
	//set status
	if (response.noBody())
	{
		if (data.dataFd[fd]->httpRequest.getConnectionClosed() || response.connectionIsClosed())
			data.dataFd[fd]->statusFd = ft::Closefd;
		else
			data.dataFd[fd]->statusFd = ft::Nosession;
	}
	else
		data.dataFd[fd]->statusFd = ft::Sendbody;
}

void	ft::Responder::_sendBody(int &fd, t_dataResp &data)
{
	// size_t		&sendByteNow = data.dataFd[fd]->sendBodyByte;
	// std::string responseBody = data.dataFd[fd]->httpResponse.getResponseBody();

	std::string responseBody = data.dataFd[fd]->httpResponse.getResponseBodyPart();
	size_t status = send(fd, responseBody.c_str(), responseBody.length(), 0);
	std::cout << "status = " << status << std::endl;
	// appendbody again
	if (data.dataFd[fd]->httpResponse.bodyIsRead())
	{
		if (data.dataFd[fd]->httpResponse.connectionIsClosed())
			data.dataFd[fd]->statusFd = ft::Closefd;
		else
			data.dataFd[fd]->statusFd = ft::Nosession;
	}
	// std::string	response = data.dataFd[fd]->responseBody.substr(sendByteNow, sendByteNow + BUF_SIZE);
	// //trim body
	// sendByteNow += response.length();
	// if (data.dataFd[fd]->responseBody.length() == sendByteNow)
	// 	data.dataFd[fd]->statusFd = ft::Closefd;
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
	// data.dataFd[fd]->responseHead = cgi.getResponseHead();
	// data.dataFd[fd]->responseBody = cgi.getResponseBody();
	data.dataFd[fd]->statusFd = ft::SendHead;

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
		case ft::Execute:
			_execute(fd, data);
			break;
		case ft::SendHead:
			_sendHead(fd, data);
			break;
		case ft::Sendbody:
			_sendBody(fd, data);
			break;
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
		data->code = HttpUtils::checkHttpRequest(*data);
		if (HttpUtils::isSuccessful(data->code))
			data->statusFd = ft::Execute;
		else
			data->statusFd = ft::SendHead;
	}
	else
		data->statusFd = ft::Readbody;
}

void ft::Responder::_get(t_dataFd *data)
{
	std::cout << "in get for " << data->httpRequest.getUrl() << std::endl;
	// 1. if a file check for reading rights ? HTTP_OK : HTTP_FORBIDDEN
	// 2. if a directory -> check for index; if no index and no autoindex -> HTTP_FORBIDDEN
	// 3. if autoindex and no reading rights; -> HTTP_FORBIDDEN
	// 4. write fd of hte file in t_dataFd;
}

void ft::Responder::_post(t_dataFd *data)
{
	std::cout << "in post for " << data->httpRequest.getUrl() << std::cout;
	// check location if it is allowed to write ? HTTP_OK : HTTP_FORBIDDEN
	// write to the location
}

void ft::Responder::_delete(t_dataFd *data)
{
	std::cout << "in delete for " << data->httpRequest.getUrl() << std::cout;

	// check location if it allowed to write if not -> HTTP_FORBIDDEN
	// Utils::delete()
}
