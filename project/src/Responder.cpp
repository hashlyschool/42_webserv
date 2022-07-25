#include "../inc/Responder.hpp"

ft::Responder::Responder()
{

}

ft::Responder::~Responder()
{

}

void	ft::Responder::action(int fd)
{
	std::string	response("HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!");
	send(fd,response.c_str(), response.length(), 0);
}
