#include "../inc/HttpRequest.hpp"

ft::HttpRequest::HttpRequest() {}

ft::HttpRequest& ft::HttpRequest::operator=(const HttpRequest & rhs) {
	this->_requestLine = rhs._requestLine;
	this->_method = rhs._method;
	this->_url = rhs._url;
	this->_httpVersion = rhs._httpVersion;
	this->_body = rhs._body;
	this->_headers.clear();
	for (std::map<std::string, std::string>::iterator it = _headers.begin();
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
	// std::cout << "method = " << _method << std::endl;
	// std::cout << "url = " << _url << std::endl;
	// std::cout << "httpVersion = " << _httpVersion << std::endl;
	// std::cout << "-------------request line end---------------"  << std::endl;
}

void	ft::HttpRequest::parseHeader(std::string requestStr) 
{
	std::string temp;

	std::stringstream requestStream(requestStr);
	parseRequestLine(requestStream);
	std::getline(requestStream, temp);

	while (!temp.empty()) // happens when \n\n encountered
	{
		size_t delimIndex = temp.find_first_of(':');
		if (delimIndex == std::string::npos)
			break; //TO DO exit invalid request
		_headers[temp.substr(0, delimIndex)] = temp.substr(delimIndex + 2); 
		// might have a lot of options, delimited with a comma -> do later
		std::getline(requestStream, temp);
	}

	// for debug
	// for (std::map<std::string, std::string>::iterator it = _headers.begin();
	// 											it != _headers.end(); it++)
	// {
	// 	std::cout << it->first << ": " << it->second << std::endl;
	// }

	// std::cout << "---------------Request line end--------------------" << std::endl;
}

bool ft::HttpRequest::isChunked()
{
	// A sender MUST NOT send a Content-Length header field 
	// in any message that contains a Transfer-Encoding header field.

	if (_headers.find("Transfer-Encoding") == _headers.end())
	{
		return false;
	}
	return (_headers["Transfer-Encoding"] == "chunked");
}

int ft::HttpRequest::getContentLength()
{
	// A user agent SHOULD NOT send a Content-Length header 
	// field when the request message does not contain a payload body 
	// and the method semantics do not anticipate such a body.

	if (_headers.find("Content-Length") == _headers.end())
		return (-1); // throw later; valid if not post or is chunked
	return _headers["Content-Length"].length();
}