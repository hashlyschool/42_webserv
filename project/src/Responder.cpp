#include "../inc/Responder.hpp"

ft::Responder::Responder()
{

}

ft::Responder::~Responder()
{

}

ft::e_statusSession	ft::Responder::action(int &fd, t_dataResp &data)
{
	//if (cgi)
		//fork
		//parsing pathinfo (to env)
		//parsing query_string (to env)
		//run script
		//return fd_file
	std::string	response("HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!");
	send(fd, response.c_str(), response.length(), 0);
	if (data.dataFd[fd])
		return (ft::Send);
	return (ft::Closefd);
}
