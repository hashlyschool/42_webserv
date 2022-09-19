#pragma once

#include <string>
#include <sstream>
#include <fstream>

// #include "./HttpRequest.hpp"
// #include "./ConfigServer.hpp"
// #include "./Responder.hpp"
#include "./Utils.hpp"
#include "./HttpUtils.hpp"

namespace ft
{
	class DataFd;

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


		private:
			unsigned int	_code;
			std::string		_method;
			std::string		_httpVersion;

			bool			_connectionClosed;
			std::string		_url;

			// ALocation	_loc;

			std::string				_bodyStr;

			/* it will be Buffer buffer*/
			char					_buf[BUF_SIZE + 1];
			size_t					_sizeOfBuf;
			unsigned long			_bodySize;
			std::string				_bodyType;
			bool					_bodyRead;
			size_t					_bytesRead; // stays
			bool					_noBody;

			std::string	_formStatusLine() const;
			void		_formPlainBody(std::string body);
			void		_setErrorPage(DataFd &data);
			std::string	_generateErrorPage();

		public:
			/* Orthodox form */
			HttpResponse();
			HttpResponse(DataFd & data); // ошибок еще нет и мы будем их проверять и записывать в t_dataFd

			~HttpResponse();

			HttpResponse &operator=(const HttpResponse &rhs);

			/* Constructors from:
				1. error -> in case of errors in the request or timeout
				2. valid request
			*/
			// void		appendBodyFromFile(std::string path);
			bool			isCGI();

			/* getters */
			const char		*getResponseBodyPart();
			size_t			getSizeOfBuf() const;
			std::string		getResponseHead() const;
			bool			bodyIsRead() const;
			bool			noBody() const;
			unsigned long	getBodySize() const;
			bool			connectionIsClosed() const;

			/* setters */
			void			setBytesRead(size_t bytes);
			void			setBodySize(size_t size);
			void			setHtml(std::string str);
			void			setBodyUrl(std::string url);
			void			setBodyType(const std::string bodyType);
	};
}
