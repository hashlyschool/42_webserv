#pragma once

#include <string>
#include <map>
#include <sys/socket.h>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include "./Utils.hpp"

namespace ft
{

	class HttpRequest
	{
		private:

			class Chunk
			{
				private:
					unsigned long	_bytesToRead;
					std::string		_chunk;
					bool			empty;
				public:
					Chunk();
					Chunk&			operator=(const Chunk & rhs);
					void			setBytesToRead(unsigned long length);
					unsigned long	getBytesToRead() const;
					std::string		getChunk() const;
					bool			isRead() const;
					bool			isEmpty() const;
					void			append(std::string string);
					void			clear();
			};

			std::string			_requestStr;
			std::string			_method;
			std::string			_url;
			std::string			_httpVersion;
			std::string			_body;
			// headers values should be stored in vector<string> ->several values for one key
			std::map< std::string, std::vector<std::string> > _headers;

			unsigned long		_contentLength;
			bool				_close;
			bool				_chunked;
			bool				_headReady;
			bool				_bodyReady;

			Chunk				_currentChunk;

			void				setContentLength();
			void				setChunked();
			void				setClose();

			bool				hasContentLength() const;

			size_t				parseRequestLine();
			void				setHeaderFields(std::string line);
			void				readBodyByChunks(std::string buffer);


		public:
			HttpRequest();
			HttpRequest&	operator=(const HttpRequest & rhs);

			/*getters*/

			std::string		getRequestStr() const;
			std::string		getURL() const;
			unsigned long	getContentLength() const;
			bool			isChunked() const;
			bool			bodyIsRead() const;
			std::string		getBody() const;
			std::string		getHttpVersion() const;
			std::string		getMethod() const;
			std::string		getUrl() const;
			bool			getConnectionClosed() const;

			/**setters**/

			void			setRequestStr(std::string source);

			/*request proccessing*/

			void			parseHeader();
			void			readBody(std::string current); // to do
			void			appendHead(std::string buf);
	};
}
