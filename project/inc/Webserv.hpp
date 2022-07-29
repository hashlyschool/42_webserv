#pragma once

#include "./Socket.hpp"
#include "./Parser.hpp"
#include "./Responder.hpp"
#include <vector>
#include <list>
#include <sys/select.h>
#include <iostream>
#include <cstdio>

namespace ft
{

	class Webserv
	{
		private:

			//Parser
			Parser		_parser;

			//Responder
			Responder	_responder;
			t_dataResp	_dataResr;

			//Select
			int			_num;
			fd_set		_mRead;
			fd_set		_mWrite;

			//Process input
			void		processStdInput();
			void		printHelp() const;

			//For serverRun
			void					createClientSocket(Socket *socket, int i);
			void					readFromClientSocket(int &fd);
			void					sendToClientSocket(int &fd);

			std::vector<Socket *>	_sockets;
			std::list<int>			_clientSocket;
		public:
			Webserv(std::string pathConf);
			~Webserv();

			void					serverRun();
	};
}
