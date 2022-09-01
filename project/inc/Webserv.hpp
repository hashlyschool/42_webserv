#pragma once

#include <vector>
#include <list>
#include <sys/select.h>
#include <iostream>
#include <cstdio>

#include "./Socket.hpp"
#include "./Parser.hpp"
#include "./Responder.hpp"


namespace ft
{

	#define MAX_TIME_CONNECTION 15
	#define MAX_CONNECTION 10

	class Webserv
	{
		private:

			//Parser
			Parser		_parser;

			//Responder
			Responder	_responder;
			t_dataResp	_dataResr;

			//Select
			int				_num;
			struct timeval	_timeout;
			fd_set			_mRead;
			fd_set			_mWrite;
			fd_set			_tWrite;
			fd_set			_tRead;

			//Process input
			void		processStdInput();
			void		printHelp() const;

			//For serverRun
			void					createClientSocket(Socket *socket, int i);
			void					readFromClientSocket(int &fd);
			void					sendToClientSocket(int &fd);
			void					fillTimeout();
			void					checkTimeConnection(int &fd);

			std::vector<Socket *>	_sockets;
			std::list<int>			_clientSocket;
			std::list<int>			_fdForDelete;
			//free memory
			void					freeMemory();
			void					removeFdClientSocket();

			// Exceptions
			class Exception : public std::exception
			{
				public:
					virtual const char* what() const throw();
			};

		public:
			Webserv(std::string pathConf);
			~Webserv();

			void					serverRun();
	};
}
