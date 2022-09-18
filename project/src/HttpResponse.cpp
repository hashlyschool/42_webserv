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
ft::HttpResponse::HttpResponse():	_code(200),
									_method(""),
									_httpVersion("HTTP/1.1"),
									_connectionClosed(false),
									_url(""),
									_bodyStr(""),
									_sizeOfBuf(0),
									_bodySize(0),
									_bodyType("text/plain"),
									_bodyRead(true),
									_bytesRead(0),
									_noBody(false)
{
	for (size_t i = 0; i < BUF_SIZE + 1; i++)
		_buf[i] = 0;
}

ft::HttpResponse::~HttpResponse() {}

ft::HttpResponse & ft::HttpResponse::operator=(const ft::HttpResponse &rhs)
{
	this->_code = rhs._code;
	this->_method = rhs._method;
	this->_httpVersion = rhs._httpVersion;
	this->_connectionClosed = rhs._connectionClosed;
	this->_url = rhs._url;
	this->_bodyStr = rhs._bodyStr;
	for (size_t i = 0; i < rhs._sizeOfBuf; i++)
	{
		this->_buf[i] = rhs._buf[i];
	}
	for (size_t i = rhs._sizeOfBuf; i < this->_sizeOfBuf; i++)
		this->_buf[i] = 0;
	this->_sizeOfBuf = rhs._sizeOfBuf;
	this->_bodySize = rhs._bodySize;
	this->_bodyType = rhs._bodyType;
	this->_bodyRead = rhs._bodyRead;
	this->_bytesRead = rhs._bytesRead;
	this->_noBody = rhs._noBody;
	return *this;
}

/* Request is taken as a valid one*/
ft::HttpResponse::HttpResponse(DataFd & data)
{
	_code = data.code;
	_method = data.httpRequest->getMethod();
	_httpVersion = data.httpRequest->getHttpVersion();
	_connectionClosed = (data.httpRequest->getConnectionClosed()
						|| _code == HTTP_REQUEST_TIMEOUT
						|| _code == HTTP_BAD_REQUEST);
	_url = "";
	_noBody = (_method == "HEAD" || _code < 200 || _code == HTTP_NO_CONTENT);
	for (size_t i = 0; i < BUF_SIZE + 1; i++)
		_buf[i] = 0;
	_sizeOfBuf = 0;
	if (_code >= 200 || _code != HTTP_NO_CONTENT)
	{
		setBodyUrl(data.finalUrl);
	}
	else
	{
		_bodySize = 0;
		_bodyType = "text/plain";
	}
	_bodyRead = false;
	_bytesRead = 0;
}

/* basic response elements construction */

std::string ft::HttpResponse::_formStatusLine() const
{
	return (_httpVersion + " " +
			Utils::to_string(_code) + " " +
			HttpUtils::getHttpReason(_code) + HEADERS_DELIMITER);
}

bool	ft::HttpResponse::isCGI()
{
	// bool	ret = false;

	// if (this.)

	return false;
}

/* getters */

const char *ft::HttpResponse::getResponseBodyPart()
{
	if (this->_url == "")
	{
		_bodyRead = true;
		_sizeOfBuf = _bodyStr.length();
		return _bodyStr.c_str();
	}
	std::ifstream file(this->_url.c_str(), std::ifstream::in | std::ifstream::binary);
	if (file.is_open() && file.good())
	{
		file.seekg(_bytesRead);
		size_t read = file.readsome(_buf, BUF_SIZE);
		_sizeOfBuf = read;
		_bytesRead += read;
		_buf[read] = '\0';
		std::cout << "read = " << read << std::endl;
		if (_bytesRead >= _bodySize)
		{
			_bodyRead = true;
		}
		file.close();
	}
	return _buf;
}

/* getters */

size_t ft::HttpResponse::getSizeOfBuf() const
{
	return _sizeOfBuf;
}

std::string ft::HttpResponse::getResponseHead() const
{
	std::stringstream headStream;
	headStream << _formStatusLine();
	if (_connectionClosed)
		headStream << "Connection: close" << HEADERS_DELIMITER;
	if (_method == "POST" && _code == HTTP_CREATED)
		headStream << "Location: " << _url << HEADERS_DELIMITER;
	if (!_noBody || _method == "HEAD")
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

bool	ft::HttpResponse::connectionIsClosed() const
{
	return false;
}

/* setters */

void	ft::HttpResponse::setBytesRead(size_t bytes)
{
	_bytesRead = bytes;
}

void	ft::HttpResponse::setBodySize(size_t size)
{
	_bodySize = size;
}

void ft::HttpResponse::setBodyUrl(std::string url)
{
	this->_url = url;
	if (!url.empty())
	{
		_bodySize = Utils::getFileSize(url);
		_bodyType = HttpUtils::getHttpFileType(url);
	}
	else
	{
		_setErrorPage();
	}
}

void	ft::HttpResponse::setBodyType(const std::string bodyType)
{
	this->_bodyType = bodyType;
}

void	ft::HttpResponse::_setErrorPage()
{
	std::string res = "";
	res += "<html><head><title>" +
	Utils::to_string(_code) +
	"</title></head><body><center><br><br><h1>Error " +
	Utils::to_string(_code) + ": " +
	HttpUtils::getHttpReason(_code) +
	"</h1><h2>Oh no! Something went wrong.</h2></center></body></html>";
	_bodyStr = res;
	_bodySize = res.length();
	_bodyType = "text/html";
}
