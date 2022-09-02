#include "../inc/HttpRequest.hpp"

ft::HttpRequest::HttpRequest() {
	_contentLength = 0;
	_chunked = false;
	_headReady = false;
	_bodyReady = false;
	_close = false;
}

ft::HttpRequest& ft::HttpRequest::operator=(const HttpRequest & rhs) {
	this->_method = rhs._method;
	this->_url = rhs._url;
	this->_httpVersion = rhs._httpVersion;
	this->_body = rhs._body;
	this->_headers.clear();
	for (std::map< std::string, std::vector<std::string> >::iterator it = _headers.begin();
												it != _headers.end(); it++)
	{
		this->_headers[it->first] = it->second;
	}
	this->_headReady = rhs._headReady;
	this->_bodyReady = rhs._bodyReady;
	this->_currentChunk = rhs._currentChunk;
	return *this;
}

size_t	ft::HttpRequest::parseRequestLine()
{
	size_t pos = 0;
	pos = ft::Utils::getdelim(_requestStr, _method, " ", pos);
	pos = ft::Utils::getdelim(_requestStr, _url, " ", pos);
	pos = ft::Utils::getdelim(_requestStr, _httpVersion, "\r\n", pos);

	// for debug
	// std::cout << "method = " << _method << std::endl;
	// std::cout << "url = " << _url << std::endl;
	// std::cout << "httpVersion = " << _httpVersion << std::endl;
	// std::cout << "-------------request line end---------------"  << std::endl;
	return pos;
}

int	ft::HttpRequest::setHeaderFields(std::string line)
{
	std::string key;
	std::string value;
	size_t pos = 0;

	pos = ft::Utils::getdelim(line, key, ": ");
	if (pos == std::string::npos)
		return -1;
	while ((pos = ft::Utils::getdelim(line, value, ",", pos)) != std::string::npos)
	{
		_headers[key].push_back(value.substr(value.find_first_not_of(' ')));
	}
	_headers[key].push_back(value.substr(value.find_first_not_of(' ')));
	return 0;
}

int	ft::HttpRequest::parseHeader()
{
	std::string temp;
	size_t pos = parseRequestLine();

	while ((pos = ft::Utils::getdelim(_requestStr, temp, "\r\n", pos)) != std::string::npos && !temp.empty())
	{
		if (setHeaderFields(temp) < 0)
		{
			return -1;
		}
	}
	if (!temp.empty())
	{
		if (setHeaderFields(temp) < 0)
			return -1;
	}
	setClose();
	if (setContentLength() < 0 || setChunked() < 0)
	{
		_bodyReady = true;
		_close = true;
		return -1;
	}

	// for debug
	for (std::map< std::string, std::vector<std::string> >::iterator it = _headers.begin();
												it != _headers.end(); it++)
	{
		std::cout << it->first << ": " << std::endl;
		std::vector<std::string> curV = it->second;
		for (size_t i = 0; i < curV.size(); i++)
		{
			std::cout << curV[i];
		}
		std::cout << std::endl;
	}

	std::cout << "---------------Request header end--------------------" << std::endl;
	return 1;
}

int ft::HttpRequest::setContentLength()
{
	if (hasContentLength())
	{
		if (_headers["Content-Length"].size() > 1)
			return (-1);
		std::string sizeStr = _headers["Content-Length"].front();
		for (size_t i = 0; i < sizeStr.length(); i++)
		{
			if (!std::isdigit(sizeStr[i]))
				return -1;
		}
		_contentLength = ft::Utils::strtoul(sizeStr, 10);
	}
	return 0;
}

int ft::HttpRequest::setChunked()
{
	if (_headers.find("Transfer-Encoding") == _headers.end())
	{
		_chunked = false;
		return 0;
	}
	std::vector<std::string> values = _headers["Transfer-Encoding"];
	if (std::find(values.begin(), values.end(), "chunked") == values.end())
		return -1;
	_chunked = true;
	return 0;
}

void ft::HttpRequest::setClose()
{
	if (_httpVersion == "HTTP/1.0")
		_close = true;
	else
		_close = false;
	if (_headers.find("Connection") != _headers.end())
	{
		std::vector<std::string> values = _headers["Connection"];
		if (std::find(values.begin(), values.end(), "keep-alive") != values.end())
			_close = false;
		else if (std::find(values.begin(), values.end(), "close") != values.end())
			_close = true;
	}
}

unsigned long ft::HttpRequest::getContentLength() const
{
	return _contentLength;
}

bool ft::HttpRequest::isChunked() const
{
	return _chunked;
}

bool ft::HttpRequest::hasContentLength() const
{
	return (_headers.find("Content-Length") != _headers.end());
}

void ft::HttpRequest::readBodyByChunks(std::string buffer)
{
	std::string lengthLine;
	size_t pos = 0;

	if (_currentChunk.isEmpty())
		pos = ft::Utils::getdelim(buffer, lengthLine, "\r\n");
	while (lengthLine != "0")
	{
		if (_currentChunk.isEmpty())
		{
			_currentChunk.setBytesToRead(ft::Utils::strtoul(lengthLine, 16));
		}
		std::string chunkBuf = buffer.substr(pos, _currentChunk.getBytesToRead());
		size_t bytesRead = chunkBuf.length();
		pos += bytesRead;

		_currentChunk.setBytesToRead(_currentChunk.getBytesToRead() - bytesRead);
		_currentChunk.append(chunkBuf);
		if (pos == buffer.length())
			break;
		if (_currentChunk.isRead())
		{
			_body.append(_currentChunk.getChunk());
			_currentChunk.clear();
			if ((pos = ft::Utils::getdelim(buffer, lengthLine, "\r\n", pos)) == std::string::npos)
				break;
		}
	}
	if (lengthLine == "0")
	{
		_bodyReady = true;
	}
}

int ft::HttpRequest::readBody(std::string current)
{
	if (_method == "DELETE" || _method == "GET")
	{
		_bodyReady = true;
		return 1;
	}
	if (isChunked())
	{
		readBodyByChunks(current);
	}
	else
	{
		size_t bytesToRead = getContentLength() - _body.length();
		_body.append(current.substr(0, bytesToRead));
		if (_body.length() == getContentLength())
			_bodyReady = true;
	}
		return 0;
}

std::string ft::HttpRequest::getBody() const
{
	return _body;
}

bool ft::HttpRequest::bodyIsRead() const
{
	return _bodyReady;
}

std::string ft::HttpRequest::getHttpVersion() const
{
	return _httpVersion;
}

std::string ft::HttpRequest::getMethod() const
{
	return _method;
}

std::string ft::HttpRequest::getUrl() const
{
	return _url;
}

void ft::HttpRequest::appendHead(std::string buf)
{
	_requestStr.append(buf);
}

std::string ft::HttpRequest::getRequestStr() const
{
	return _requestStr;
}

std::string ft::HttpRequest::getURL() const
{
	return _url;
}

bool ft::HttpRequest::getConnectionClosed() const
{
	return _close;
}

void ft::HttpRequest::setRequestStr(std::string source)
{
	_requestStr = source;
}

/* Chunk class definition */

ft::HttpRequest::Chunk::Chunk(): empty(true) {}

ft::HttpRequest::Chunk& ft::HttpRequest::Chunk::operator=(const Chunk & rhs)
{
	this->_bytesToRead = rhs._bytesToRead;
	this->_chunk = rhs._chunk;
	this->empty = rhs.empty;
	return *this;
}

void ft::HttpRequest::Chunk::setBytesToRead(unsigned long bytesToRead)
{
	_bytesToRead = bytesToRead;
	empty = false;
}

unsigned long ft::HttpRequest::Chunk::getBytesToRead() const
{
	return _bytesToRead;
}

std::string	 ft::HttpRequest::Chunk::getChunk() const
{
	return _chunk;
}

bool ft::HttpRequest::Chunk::isRead() const
{
	return (_bytesToRead == 0 && !empty);
}

bool ft::HttpRequest::Chunk::isEmpty() const
{
	return empty;
}

void ft::HttpRequest::Chunk::append(std::string string)
{
	_chunk.append(string);
}

void ft::HttpRequest::Chunk::clear()
{
	_bytesToRead = 0;
	_chunk.clear();
	empty = true;
}
