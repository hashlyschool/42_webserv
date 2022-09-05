#include "../inc/DataFd.hpp"

ft::DataFd::DataFd()
{
	this->code = 200;
	this->finalUrl = "";
	this->statusFd = ft::Nosession;
	this->httpRequest = new HttpRequest();
	this->httpResponse = new HttpResponse();
	gettimeofday(&this->timeLastAction, NULL);
}

ft::DataFd::~DataFd()
{
	if (this->httpRequest)
		delete this->httpRequest;
	if (this->httpResponse)
		delete this->httpResponse;
}

void	ft::DataFd::updateTime()
{
	if (cgi.hasChildProcess())
		return ;
	gettimeofday(&timeLastAction, NULL);
}
