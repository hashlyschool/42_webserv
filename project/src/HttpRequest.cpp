#include "../inc/HttpRequest.hpp"

ft::HttpRequest::HttpRequest() {}

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
	return *this;
}

void	ft::HttpRequest::parseRequestLine(std::stringstream &requestStream)
{
	//todo error handling
	std::getline(requestStream, _method, ' ');
	std::getline(requestStream, _url, ' ');
	std::getline(requestStream, _httpVersion);

	// for debug
	std::cout << "method = " << _method << std::endl;
	std::cout << "url = " << _url << std::endl;
	std::cout << "httpVersion = " << _httpVersion << std::endl;
	std::cout << "-------------request line end---------------"  << std::endl;
}

void	ft::HttpRequest::parseHeader(std::string requestStr) 
{
	std::string temp;

	std::stringstream requestStream(requestStr);
	parseRequestLine(requestStream);
	std::getline(requestStream, temp);
	

	while (!temp.empty()) // happens when \n\n encountered
	{
		
		std::stringstream curLine(temp);
		std::string key;
		std::string value;

		std::getline(curLine, key, ':');
		while (std::getline(curLine, value, ','))
		{
			_headers[key].push_back(value.substr(value.find_first_not_of(' ')));
		}
		std::getline(requestStream, temp);
	}

	//make a method fill in fields
	if (hasContentLength())
	{
		std::string sizeStr = _headers["Content-Length"].front();
		char *startString = &(sizeStr[0]);
		char *endString = &(sizeStr[sizeStr.length()]);

		_contentLength = std::strtoul(startString, &endString, 10);
	}

	// for debug
	for (std::map< std::string, std::vector<std::string> >::iterator it = _headers.begin();
												it != _headers.end(); it++)
	{
		std::cout << it->first << ": " << std::endl;
		std::vector<std::string> curV = it->second;
		for (size_t i = 0; i < curV.size(); i++)
		{
			std::cout << curV[i] << "";
		}
		std::cout << std::endl;
	}

	std::cout << "---------------Request header end--------------------" << std::endl;
}

bool ft::HttpRequest::isChunked()
{
	// A sender MUST NOT send a Content-Length header field 
	// in any message that contains a Transfer-Encoding header field.

	if (_headers.find("Transfer-Encoding") == _headers.end())
	{
		return false;
	}
	std::vector<std::string> values = _headers["Transfer-Encoding"];
	return (std::find(values.begin(), values.end(), "chunked") != values.end());
}

unsigned long ft::HttpRequest::getContentLength()
{
	// A user agent SHOULD NOT send a Content-Length header 
	// field when the request message does not contain a payload body 
	// and the method semantics do not anticipate such a body.
	return _contentLength;
}

bool ft::HttpRequest::hasContentLength()
{
	return (_headers.find("Content-Length") != _headers.end());
}