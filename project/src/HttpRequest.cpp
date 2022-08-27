#include "../inc/HttpRequest.hpp"

ft::HttpRequest::HttpRequest() {
	_contentLength = 0;
	_chunked = false;
	_headReady = false;
	_bodyReady = false;
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
	//todo error handling
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

void	ft::HttpRequest::setHeaderFields(std::string line)
{
	std::string key;
	std::string value;
	size_t pos = 0;

	pos = ft::Utils::getdelim(line, key, ": ");
	while ((pos = ft::Utils::getdelim(line, value, ",", pos)) != std::string::npos)
	{
		_headers[key].push_back(value.substr(value.find_first_not_of(' ')));
	}
	_headers[key].push_back(value.substr(value.find_first_not_of(' ')));
}

void	ft::HttpRequest::parseHeader()
{
	std::string temp;
	size_t pos = parseRequestLine();
	// std::getline(requestStream, temp);

	while ((pos = ft::Utils::getdelim(_requestStr, temp, "\r\n", pos)) != std::string::npos && !temp.empty())
	{
		setHeaderFields(temp);
	}
	if (!temp.empty())
		setHeaderFields(temp);
	setContentLength();
	setChunked();

	// for debug
	// for (std::map< std::string, std::vector<std::string> >::iterator it = _headers.begin();
	// 											it != _headers.end(); it++)
	// {
	// 	std::cout << it->first << ": " << std::endl;
	// 	std::vector<std::string> curV = it->second;
	// 	for (size_t i = 0; i < curV.size(); i++)
	// 	{
	// 		std::cout << curV[i];
	// 	}
	// 	std::cout << std::endl;
	// }

	// std::cout << "---------------Request header end--------------------" << std::endl;
}

void ft::HttpRequest::setContentLength()
{
	if (hasContentLength())
	{
		std::string sizeStr = _headers["Content-Length"].front();
		_contentLength = ft::Utils::strtoul(sizeStr, 10);
	}
}

void ft::HttpRequest::setChunked()
{
	if (_headers.find("Transfer-Encoding") == _headers.end())
	{
		_chunked = false;
		return;
	}
	std::vector<std::string> values = _headers["Transfer-Encoding"];
	_chunked = (std::find(values.begin(), values.end(), "chunked") != values.end());
}


unsigned long ft::HttpRequest::getContentLength() const
{
	// A user agent SHOULD NOT send a Content-Length header
	// field when the request message does not contain a payload body
	// and the method semantics do not anticipate such a body.

	return _contentLength;
}

bool ft::HttpRequest::isChunked() const
{
	// A sender MUST NOT send a Content-Length header field
	// in any message that contains a Transfer-Encoding header field.

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

void ft::HttpRequest::readBody(std::string current)
{
	if (_method == "DELETE" || _method == "GET")
	{
		_bodyReady = true;
		return;
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
}

std::string ft::HttpRequest::getBody() const
{
	return _body;
}

bool ft::HttpRequest::bodyIsRead() const
{
	return _bodyReady;
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
