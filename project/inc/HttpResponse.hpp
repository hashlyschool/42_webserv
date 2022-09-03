#pragma once

#include <string>
#include <sstream>
#include <fstream>

#include "./HttpRequest.hpp"
#include "./ConfigServer.hpp"
#include "./Responder.hpp"
#include "./Utils.hpp"
#include "./HttpUtils.hpp"

namespace ft
{
	class HttpResponse
	{
		/*
			errors right after request:
			- check for closed socket and if the body or header not read yet -> error
			in Server maybe

		*/

		/* invalid cases:
		if there is a whitespace between start line and next line
		-> either reject the request or skip the following line

		if no host -> case invalid
		*/

		#define HEADERS_DELIMITER "\r\n"

		private:
			unsigned int	_code;
			std::string		_method;
			std::string		_httpVersion;

			bool			_connectionClosed;
			std::string		_url;

			// ALocation	_loc;

			std::string		_bodyStr;
			unsigned long	_bodySize;
			std::string		_bodyType;
			bool			_bodyRead;
			size_t			_bytesRead; // stays
			bool			_noBody;

			std::string	_formStatusLine() const;
			void		_formPlainBody(std::string body);

		public:
			/* Orthodox form */
			HttpResponse();
			HttpResponse &operator=(const HttpResponse &rhs);
			~HttpResponse();

			/* Constructors from:
				1. error -> in case of errors in the request or timeout
				2. valid request
			*/

			HttpResponse(t_dataFd & data); // ошибок еще нет и мы будем их проверять и записывать в t_dataFd

			void		setBodyUrl(std::string url);
			// void		appendBodyFromFile(std::string path);

			/* getters */
			std::string		getResponseHead() const;
			std::string		getResponseBodyPart();
			bool			connectionIsClosed() const;
			bool			bodyIsRead() const;
			bool			noBody() const;
			unsigned long	getBodySize() const;
	};
}
