#pragma once

#include "./Socket.hpp"
#include "./Parser.hpp"
#include "./Responder.hpp"
#include <vector>
#include <list>
#include <sys/select.h>
#include <limits>
#include <iostream>
#include <cstdio>

namespace ft
{

	class Webserv
	{
		private:
			Parser					_parser;
			std::vector<Socket *>	_sockets;
			std::list<int>			_clientSocket;
			Responder				_responder;

			int		_num;
			fd_set	_mRead;
			fd_set	_mWrite;

			//Process input
			void	processStdInput();
			void	printHelp() const;

			//For serverRiun
			void	createClientSocket(Socket *socket);
		public:
			Webserv(std::string pathConf);
			~Webserv();

			void					serverRun();
	};
}
