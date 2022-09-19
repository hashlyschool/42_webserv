#include "../inc/DataFd.hpp"

ft::DataFd::DataFd(int fd)
{
	this->fd = fd;
	this->code = 200;
	this->finalUrl = "";
	this->statusFd = ft::Nosession;
	this->httpRequest = new HttpRequest();
	this->httpResponse = new HttpResponse();
	this->cgi = new Cgi();
	this->outFile = NULL;
	this->loc = NULL;
	gettimeofday(&this->_timeLastAction, NULL);
}

ft::DataFd::~DataFd()
{
	if (this->httpRequest)
		delete this->httpRequest;
	if (this->httpResponse)
		delete this->httpResponse;
	if (this->cgi)
		delete this->cgi;
	if (outFile)
	{
		if (outFile->is_open())
			outFile->close();
		delete outFile;
	}
}

void	ft::DataFd::clear()
{
	this->code = 200;
	this->finalUrl = "";
	*(this->httpRequest) = HttpRequest();
	*(this->httpResponse) = HttpResponse();
	if (outFile)
	{
		if (outFile->is_open())
			outFile->close();
		delete outFile;
		outFile = NULL;
	}
}

void	ft::DataFd::updateTime()
{
	if (cgi->hasChildProcess())
		return ;
	gettimeofday(&_timeLastAction, NULL);
}

struct timeval	ft::DataFd::getTimeLastAct() const { return (_timeLastAction); }
