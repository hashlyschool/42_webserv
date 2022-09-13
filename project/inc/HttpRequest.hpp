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
#include "./Buffer.hpp"

namespace ft
{
	/*
	errors that should be caught while parsing HttpRequest:
	- bad http version -> we might not know how to parse it
	- bad http request -> in case of syntaxys errors? maybe security reasons but i'm not sure
	- header too long - in request for security reasons
	- transfer_enc && !chunked final - in request beacuse we don't know how to read this
	- multiple different content-length or invalid content-length - in request beacuse we don't know how to read this
	- limit size of chunks -> if the size of a chunk exceeds it -> error - in request because it is a reading body error
*/

	class HttpRequest
	{
		private:

			class Chunk
			{
				private:
					unsigned long	_bytesToRead;
					Buffer			_chunk;
					bool			empty;
					std::string		_lengthLineTmp;
				public:
					Chunk();
					Chunk&			operator=(const Chunk & rhs);
					void			setBytesToRead(std::string line);
					void			setBytesToRead(unsigned long length);
					unsigned long	getBytesToRead() const;
					const Buffer	&getChunk() const;
					bool			isRead() const;
					bool			isEmpty() const;
					void			append(Buffer buf);
					void			clear();
					void			setLengthLineTmp(const std::string & line);
			};

			std::string			_requestStr;
			std::string			_method;
			std::string			_url;
			std::string			_httpVersion;
			// std::string			_body;
			// headers values should be stored in vector<string> ->several values for one key
			std::map< std::string, std::vector<std::string> > _headers;

			// Body Buffer fields
			Buffer				_buffer;
			size_t				_totalBytesRead;

			size_t				_bytesToRead;
			unsigned long		_contentLength;


			bool				_close;
			bool				_chunked;
			bool				_headReady;
			bool				_bodyReady;

			Chunk				_currentChunk;

			int					setContentLength();
			int					setChunked();
			void				setClose();

			bool				hasContentLength() const;

			size_t				parseRequestLine();
			int					setHeaderFields(std::string line);
			void				readBodyByChunks(char *current, size_t sizeBuf);

		public:
			HttpRequest();
			HttpRequest&	operator=(const HttpRequest & rhs);

			/*getters*/

			std::string		getRequestStr() const;
			unsigned long	getContentLength() const;
			bool			isChunked() const;
			bool			bodyIsRead() const;
			const char		*getBody() const;
			size_t			getBodySize() const;
			std::string		getHttpVersion() const;
			std::string		getMethod() const;
			std::string		getUrl() const;
			bool			getConnectionClosed() const;

			/**setters**/

			void			setRequestStr(std::string source);

			/*request proccessing*/

			int				parseHeader();
			int				readBody(char *current, size_t size);
			void			setHead(std::string buf);
	};
}
