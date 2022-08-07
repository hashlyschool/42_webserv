#include <string>
#include <map>
#include <sys/socket.h>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <algorithm>

namespace ft
{

	class HttpRequest
	{

		private:
			std::string			_requestStr;
			std::string			_method;
			std::string			_url;
			std::string			_httpVersion;
			std::string			_body;
			// headers values should be stored in vector<string> ->several values for one key
			std::map< std::string, std::vector<std::string> > _headers;
			unsigned long		_contentLength;
			// bool				_chunked;

			void				parseRequestLine(std::stringstream &requestStream);

		public:
			HttpRequest();
			HttpRequest&	operator=(const HttpRequest & rhs);
			void			parseHeader(std::string requestStr);
			unsigned long	getContentLength();
			bool			isChunked();
			bool			hasContentLength();
			void			getBody(char *body); // to do
	};
}
