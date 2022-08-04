#include <string>
#include <map>
#include <sys/socket.h>
#include <sstream>
#include <iostream>

namespace ft
{
	#define BUF_SIZE 2048

	class HttpRequest
	{

		private:
			std::string			_requestLine;
			std::string			_method;
			std::string			_url;
			std::string			_httpVersion;
			std::string			_body;
			std::map<std::string, std::string> _headers;
			// unsigned int		_contentLength;
			// bool				_chunked;
			void				parseRequestLine(std::stringstream &requestStream);

		public:
			HttpRequest();
			HttpRequest&	operator=(const HttpRequest & rhs);
			void			parseHeader(std::string requestStr);
			int				getContentLength();
			bool			isChunked();
			void			getBody(char *body); // to do
	};
}
