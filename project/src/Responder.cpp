#include "../inc/Responder.hpp"

ft::Responder::Responder() {}

ft::Responder::~Responder() {}

void	ft::Responder::_makeSession(int &fd, t_dataResp &data)
{
	int			status;
	char		buf[BUF_SIZE];
	std::string	temp;
	size_t		startBody;

	status = recv(fd, buf, BUF_SIZE, 0);
	temp = buf;
	startBody = temp.find("\r\n\r\n"); //\n\n
	if (startBody != temp.npos)
	{
		data.dataFd[fd]->requestHead = temp.substr(0, startBody);
		data.dataFd[fd]->requestBody = temp.substr(startBody, temp.length());
	}
	else
		data.dataFd[fd]->requestHead.append(temp);
	//set status
	if (status < BUF_SIZE)
		data.dataFd[fd]->statusFd = ft::Send;
	else
		data.dataFd[fd]->statusFd = ft::Readbody;
	// std::cout << "----------------\n";
	// std::cout << buf;
	// std::cout << "----------------\n";
}

void	ft::Responder::_readBody(int &fd, t_dataResp &data)
{
	int		status;
	char	buf[BUF_SIZE];

	status = recv(fd, buf, BUF_SIZE, 0);
	data.dataFd[fd]->requestBody.append(buf);
	//set status
	if (status == 0)
		data.dataFd[fd]->statusFd = ft::Send;
	else
		data.dataFd[fd]->statusFd = ft::Readbody;
}

void	ft::Responder::_send(int &fd, t_dataResp &data)
{
	int	status;

	//create response head
	data.dataFd[fd]->responseHead = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\n";
	//create response body
	data.dataFd[fd]->responseBody = "Hello world!";
	//send Head
	std::string &response = data.dataFd[fd]->responseHead;
	status = send(fd, response.c_str(), response.length(), 0);
	std::cout << "status = " << status << std::endl;
	//set status
	if (data.dataFd[fd]->responseBody.length() == 0)
		data.dataFd[fd]->statusFd = ft::Closefd;
	else
		data.dataFd[fd]->statusFd = ft::Sendbody;
}

void	ft::Responder::_sendBody(int &fd, t_dataResp &data)
{
	int		status;
	size_t		&sendByteNow = data.dataFd[fd]->sendBodyByte;

	//send Body
	std::string	response = data.dataFd[fd]->responseBody.substr(sendByteNow, sendByteNow + BUF_SIZE);
	status = send(fd, response.c_str(), response.length(), 0);
	std::cout << "status = " << status << std::endl;
	//trim body
	sendByteNow += response.length();
	if (data.dataFd[fd]->responseBody.length() == sendByteNow)
		data.dataFd[fd]->statusFd = ft::Closefd;
}

void	ft::Responder::_cgi(int &fd, t_dataResp &data)
{
	if (fd || data.dataFd[fd]->statusFd)
		fd = fd;
}

void	ft::Responder::_closeFd(int &fd, t_dataResp &data)
{
	if (fd || data.dataFd[fd]->statusFd)
		fd = fd;
}

void	ft::Responder::_autoIndex(int &fd, t_dataResp &data)
{
	if (fd || data.dataFd[fd]->statusFd)
		fd = fd;
}

void	ft::Responder::action(int &fd, t_dataResp &data)
{
	int	status = data.dataFd[fd]->statusFd;

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
		case ft::Cgi:
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
