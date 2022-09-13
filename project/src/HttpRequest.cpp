#include "../inc/HttpRequest.hpp"

ft::HttpRequest::HttpRequest() {
	_buffer = Buffer();
	_totalBytesRead = 0;
	_bytesToRead = 0;
	_contentLength = 0;
	_chunked = false;
	_headReady = false;
	_bodyReady = false;
	_close = false;
}

ft::HttpRequest& ft::HttpRequest::operator=(const HttpRequest & rhs) {
	this->_requestStr = rhs._requestStr;
	this->_method = rhs._method;
	this->_url = rhs._url;
	this->_httpVersion = rhs._httpVersion;
	this->_headers.clear();
	for (std::map< std::string, std::vector<std::string> >::iterator it = _headers.begin();
												it != _headers.end(); it++)
	{
		this->_headers[it->first] = it->second;
	}
	this->_contentLength = rhs._contentLength;
	this->_close = rhs._close;
	this->_chunked = rhs._chunked;
	this->_headReady = rhs._headReady;
	this->_bodyReady = rhs._bodyReady;
	this->_currentChunk = rhs._currentChunk;
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

void ft::HttpRequest::readBodyByChunks(char *current, size_t sizeBuf)
{
	std::string lengthLine; // "delimiter for a chunk == its size in 0x"
	size_t pos = 0; // start of body of the chunk

	if (_currentChunk.isEmpty())
	{
		pos = ft::Utils::getdelim(current, lengthLine, "\r\n");
		if (pos == std::string::npos) // we didn't get the whole lengthString in buf
		{
			_currentChunk.setLengthLineTmp(lengthLine);
			return;
		}

	}
	while (lengthLine != "0")
	{
		if (_currentChunk.isEmpty())
		{
			_currentChunk.setBytesToRead(lengthLine);
		}
		// bytesToRead = number of characters to read from input buffer
		// but the buffer can be smaller than the current size of chunk
		size_t bytesToRead = (sizeBuf - pos > _currentChunk.getBytesToRead()) ?
		_currentChunk.getBytesToRead() : sizeBuf - pos;
		Buffer chunkBuf(current + pos, bytesToRead);
		// std::string chunkBuf = current.substr(pos, _currentChunk.getBytesToRead());
		pos += bytesToRead;

		_currentChunk.setBytesToRead(_currentChunk.getBytesToRead() - bytesToRead);
		_currentChunk.append(chunkBuf);
		if (pos == sizeBuf) // we've read everything from the buffer
			return;
		if (_currentChunk.isRead())
		{
			_buffer.append(_currentChunk.getChunk());
			_currentChunk.clear();
			if ((pos = ft::Utils::getdelim(current, lengthLine, "\r\n", pos)) == std::string::npos)
			{
				_currentChunk.setLengthLineTmp(lengthLine);
				return;
			}
		}
	}
	_bodyReady = true;
}

int ft::HttpRequest::readBody(char *current, size_t sizeBuf)
{
	if (_method == "DELETE" || _method == "GET")
	{
		_bodyReady = true;
		return 1;
	}
	if (isChunked())
	{
		readBodyByChunks(current, sizeBuf);
	}
	else
	{
		_buffer = Buffer(current, sizeBuf);
		_totalBytesRead += sizeBuf;
		std::cout << "read " << _totalBytesRead
				<< " length " << _contentLength << "\n";
		if (_totalBytesRead >= _contentLength)
		{
			std::cout << "I decided that the body is done\n";
			_bodyReady = true;
		}
	}
		return 0;
}

const char *ft::HttpRequest::getBody() const
{
	return _buffer.getBuffer();
}

size_t ft::HttpRequest::getBodySize() const
{
	return _buffer.size();
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

void ft::HttpRequest::setHead(std::string buf)
{
	_requestStr.append(buf);
}

std::string ft::HttpRequest::getRequestStr() const
{
	return _requestStr;
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

ft::HttpRequest::Chunk::Chunk(): _bytesToRead(0),
								_chunk(Buffer()),
								empty(true),
								_lengthLineTmp("") {}

ft::HttpRequest::Chunk& ft::HttpRequest::Chunk::operator=(const Chunk & rhs)
{
	this->_lengthLineTmp = rhs._lengthLineTmp;
	this->_bytesToRead = rhs._bytesToRead;
	this->_chunk = rhs._chunk;
	this->empty = rhs.empty;
	return *this;
}

void ft::HttpRequest::Chunk::setBytesToRead(std::string line)
{
	_lengthLineTmp += line;

	_bytesToRead = Utils::strtoul(_lengthLineTmp, 16);
	empty = false;
}

void ft::HttpRequest::Chunk::setBytesToRead(unsigned long length)
{
	_bytesToRead = length;
}

unsigned long ft::HttpRequest::Chunk::getBytesToRead() const
{
	return _bytesToRead;
}

const ft::Buffer &ft::HttpRequest::Chunk::getChunk() const
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

void ft::HttpRequest::Chunk::append(Buffer buf)
{
	_chunk.append(buf);
}

void ft::HttpRequest::Chunk::clear()
{
	_bytesToRead = 0;
	_chunk = Buffer();
	empty = true;
	_lengthLineTmp = "";
}

void ft::HttpRequest::Chunk::setLengthLineTmp(const std::string & line)
{
	_lengthLineTmp = line;
}
