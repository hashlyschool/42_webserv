#include "../inc/DataFd.hpp"

ft::DataFd::DataFd()
{
	this->httpRequest = new HttpRequest();
	this->httpResponse = new HttpResponse();
}

ft::DataFd::~DataFd()
{
	if (this->httpRequest)
		delete this->httpRequest;
	if (this->httpResponse)
		delete this->httpResponse;
}
