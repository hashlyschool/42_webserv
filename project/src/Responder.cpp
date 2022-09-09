#include "../inc/Responder.hpp"

ft::Responder::Responder() {}

ft::Responder::~Responder() {}

void	ft::Responder::_makeSession(int &fd, DataFd &data)
{
	std::string	input;
	std::string inputHeader;
	size_t		startBody;

	data.clear();
	HttpRequest &curRequest = *data.httpRequest;

	input = ft::Utils::readFromSocket(fd, BUF_SIZE);
	startBody = ft::Utils::getdelim(input, inputHeader, DELIMITER);
	curRequest.appendHead(inputHeader);
	if (startBody != std::string::npos)
	{
		if (curRequest.parseHeader() < 0)
			data.code = HTTP_BAD_REQUEST;
		if (!curRequest.getConnectionClosed())
			curRequest.readBody(input.substr(startBody));
		_setStatusRequest(&data);
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
			data.code = HTTP_BAD_REQUEST;
		curRequest.readBody(body);
		_setStatusRequest(&data);
	}
}

void	ft::Responder::_readBody(int &fd, DataFd &data)
{
	std::string input = ft::Utils::readFromSocket(fd, BUF_SIZE);

	data.httpRequest->readBody(input);
	//set status
	_setStatusRequest(&data);
}

void ft::Responder::_execute(DataFd &data)
{
	//create response head
	// std::cout << "body = " << data.dataFd[fd]->httpRequest.getBody() << std::endl;
	// std::cout << "\nREQUEST\n\n" << data.dataFd[fd]->httpRequest.getRequestStr() << "\n\n";
	// std::cout << "\nURL\n\n" << data.dataFd[fd]->httpRequest.getURL() << "\n\n";
	std::string method = data.httpRequest->getMethod();
	if (method == "GET")
		_get(&data);
	else if (method == "POST")
		_post(&data);
	else
		_delete(&data);
	data.statusFd = ft::SendHead;
}

void	ft::Responder::_sendHead(int &fd, DataFd &data)
{
	int	status;

	//create response head
	HttpResponse &response = *data.httpResponse;
	response = HttpResponse(data);

	response.setBodyUrl(data.finalUrl);
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
		if (data.httpRequest->getConnectionClosed() || response.connectionIsClosed())
			data.statusFd = ft::Closefd;
		else
			data.statusFd = ft::Nosession;
	}
	else
		data.statusFd = ft::Sendbody;
}

void	ft::Responder::_sendBody(int &fd, DataFd &data)
{
	// size_t		&sendByteNow = data.dataFd[fd]->sendBodyByte;
	// std::string responseBody = data.dataFd[fd]->httpResponse.getResponseBody();

	const char *responseBody = data.httpResponse->getResponseBodyPart();
	size_t status = send(fd, responseBody, data.httpResponse->getSizeOfBuf(), 0);
	std::cout << "SendBody status = " << status << std::endl;
	// appendbody again
	if (data.httpResponse->bodyIsRead())
	{
		if (data.httpResponse->connectionIsClosed())
			data.statusFd = ft::Closefd;
		else
			data.statusFd = ft::Nosession;
	}
	// std::string	response = data.dataFd[fd]->responseBody.substr(sendByteNow, sendByteNow + BUF_SIZE);
	// //trim body
	// sendByteNow += response.length();
	// if (data.dataFd[fd]->responseBody.length() == sendByteNow)
	// 	data.dataFd[fd]->statusFd = ft::Closefd;
}

void	ft::Responder::_cgi(DataFd &data)
{
	int	status;

	if (data.cgi->hasChildProcess())
		data.cgi->waitChildProcess(data);
	else
	{
		status = data.cgi->isCGI(data);
		if (status == 0)
			data.statusFd = ft::Execute;
		else if (status > 0)
			data.cgi->runChildProcess(data);
		else
			data.statusFd = ft::SendHead;
	}
}

void	ft::Responder::_closeFd(int &fd, MapDataFd &data)
{
	std::cout << "[INFO] Close fd = " << fd << "\n";
	close(fd);
	delete data[fd];
	data.erase(fd);
	//if connection close or time -> makesession
	//else close
}

void	ft::Responder::_autoIndex(int &fd, MapDataFd &data)
{
	if (fd || data[fd]->statusFd)
		fd = fd;
}

void	ft::Responder::action(int &fd, MapDataFd &data)
{
	DataFd	&dataFd = *data[fd];
	int		status = dataFd.statusFd;

	dataFd.updateTime();
	switch (status)
	{
		case ft::Nosession:
			_makeSession(fd, dataFd);
			break;
		case ft::Readbody:
			_readBody(fd, dataFd);
			break;
		case ft::Execute:
			_execute(dataFd);
			break;
		case ft::SendHead:
			_sendHead(fd, dataFd);
			break;
		case ft::Sendbody:
			_sendBody(fd, dataFd);
			break;
		case ft::CGI:
			_cgi(dataFd);
			break;
		case ft::Closefd:
			_closeFd(fd, data);
			break;
		case ft::AutoIndex:
			_autoIndex(fd, data);
			break;
	}
}

void ft::Responder::_setStatusRequest(DataFd *data)
{
	if (data->httpRequest->bodyIsRead())
	{
		data->code = HttpUtils::checkHttpRequest(*data);
		if (HttpUtils::isSuccessful(data->code))
		{
			if (data->loc->getIsCgi())
				data->statusFd = ft::CGI;
			else
				data->statusFd = ft::Execute;
		}
		else
			data->statusFd = ft::SendHead;
	}
	else
		data->statusFd = ft::Readbody;
}

void ft::Responder::_get(DataFd *data)
{
	std::cout << "in get for " << data->httpRequest->getUrl() << std::endl;
	const ALocation * loc = data->loc;
	if (!loc->getIsGet())
	{
		data->code = HTTP_METHOD_NOT_ALLOWED;
		return;
	}
	std::string url = data->configServer->getFilename(data->httpRequest->getUrl(), *loc);
	std::cout << "found url: " << url << std::endl;
	if (!Utils::fileExists(url))
	{
		data->code = HTTP_NOT_FOUND;
		return;
	}
	if (!Utils::fileIsReadable(url))
	{
		data->code = HTTP_FORBIDDEN;
		return;
	}
	if (Utils::isDirectory(url))
	{
		if (data->configServer->getFilename((url + loc->getIndex()), *loc).empty())
		{
			if (!loc->getAutoIndex())
			{
				data->code = HTTP_FORBIDDEN;
				return;
			}
			else
			{
				std::cout << "It will be autoindex once it is ready" << std::endl;
			}
		}
		else
		{
			url += loc->getIndex();
		}
	}
	data->finalUrl = url;
}

void ft::Responder::_post(DataFd *data)
{
	std::cout << "in post for " << data->httpRequest->getUrl() << std::endl;
	// check location if it is allowed to write ? HTTP_OK : HTTP_FORBIDDEN
	// write to the location
}

void ft::Responder::_delete(DataFd *data)
{
	std::cout << "in delete for " << data->httpRequest->getUrl() << std::endl;
	const ALocation * loc = data->loc;
	std::string url = data->configServer->getFilename(data->httpRequest->getUrl(), *loc);

	if (!loc->getIsDelete())
		data->code = HTTP_METHOD_NOT_ALLOWED;
	else if (Utils::isNotEmptyDirectory(url))
		data->code = HTTP_CONFLICT;
	else if (!Utils::fileExists(url))
		data->code = HTTP_NOT_FOUND;
	else if (!Utils::fileIsWritable(url))
		data->code = HTTP_FORBIDDEN;
	else if (std::remove(url.c_str()) != 0)
		data->code = HTTP_INTERNAL_SERVER_ERROR;
	else
		data->code = HTTP_NO_CONTENT;
}
