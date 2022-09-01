#include "../inc/HttpResponse.hpp"

/*
*	Minimal response:
*	HTTP/1.1 200 OK
*	Date: Mon, 27 Jul 2009 12:28:53 GMT
*	Server: Apache/2.2.14 (Win32)
*	Last-Modified: Wed, 22 Jul 2009 19:15:56 GMT
*	Content-Length: 88
*	Content-Type: text/html
*	Connection: Closed
*
*/

/* Orthodox form */
ft::HttpResponse::HttpResponse() {}

ft::HttpResponse & ft::HttpResponse::operator=(const ft::HttpResponse &rhs)
{
	this->_code = rhs._code;
	this->_method = rhs._method;
	this->_httpVersion = rhs._httpVersion;
	this->_connectionClosed = rhs._connectionClosed;
	this->_url = rhs._url;
	this->_bodySize = rhs._bodySize;
	this->_bodyType = rhs._bodyType;
	this->_bodyRead = rhs._bodyRead;
	this->_bytesRead = rhs._bytesRead;
	this->_noBody = rhs._noBody;
}

/* Request is taken as a valid one*/
ft::HttpResponse::HttpResponse(t_dataFd & data)
{
	_code = data.statusFd;
	_method = data.httpRequest.getMethod();
	_httpVersion = data.httpRequest.getHttpVersion();
	_connectionClosed = data.httpRequest.getConnectionClosed();
	// _url = data.configServer->getLocation().getUrl();
	_noBody = (data.httpRequest.getMethod() == "HEAD" || _code < 200 || _code == HTTP_NO_CONTENT);
	if (!_noBody && _method == "GET") //or cgi; to do later
	{
		_bodySize = Utils::getFileSize(_url);
		_bodyType = HttpUtils::getHttpFileType(_url);
	}
	else
	{
		_bodySize = 0;
		_bodyType = "text/plain";
	}
	_bodyRead = false;
	_bytesRead = false;
}

/* basic response elements construction */

std::string ft::HttpResponse::_formStatusLine() const
{
	return (_httpVersion + " " +
			Utils::to_string(_code) + " " +
			HttpUtils::getHttpReason(_code) + HEADERS_DELIMITER);
}

std::string ft::HttpResponse::getResponseBodyPart()
{
	if (this->_url == "")
	{
		if (_code < 200 || _code > 299)
			return HttpUtils::getHttpReason(_code);
		return "";
	}
	std::ifstream file(this->_url);
	char buf[BUF_SIZE + 1];
	if (file.is_open() && file.good())
	{
		file.seekg(_bytesRead);
		size_t read = file.readsome(buf, BUF_SIZE);
		_bytesRead += read;
		buf[read] = '\0';
		if (_bytesRead == _bodySize)
			_bodyRead = true;
		file.close();
	}
	return std::string(buf);
}

void ft::HttpResponse::setBodyUrl(std::string url)
{
	if (_method == "GET")
	{
		this->_url = url;
		_bodySize = Utils::getFileSize(url);
		_bodyType = HttpUtils::getHttpFileType(url);
	}
}

/* getters */

std::string ft::HttpResponse::getResponseHead() const
{
	std::stringstream headStream;
	headStream << _formStatusLine();
	if (_connectionClosed)
		headStream << "Connection: close" << HEADERS_DELIMITER;
	if (!_noBody)
	{
		headStream <<"Content-Type: " << _bodyType << HEADERS_DELIMITER;
		headStream << "Content-Length: " << _bodySize << HEADERS_DELIMITER;
	}
	headStream << HEADERS_DELIMITER;
	return headStream.str();
}

bool ft::HttpResponse::bodyIsRead() const
{
	return _bodyRead;
}

bool ft::HttpResponse::noBody() const
{
	return _noBody;
}

unsigned long ft::HttpResponse::getBodySize() const
{
	return _bodySize;
}