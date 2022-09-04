#include "../inc/DataFd.hpp"

ft::DataFd::DataFd()
{
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
