#pragma once

#include <unistd.h>
#include <string>
#include <map>
#include <sys/socket.h>
#include <cstdlib>
#include "./ConfigServer.hpp"
#include "./HttpRequest.hpp"
#include "./Cgi.hpp"
#include "./Utils.hpp"

namespace ft
{
	#define BUF_SIZE	2048
	#define DELIMITER	"\r\n\r\n"

	enum e_statusSession
	{
		Nosession,	//только создан, прочитать head, обработать
		Readbody,	//идет чтение body request
		Send,		//Чтение закончено, нужно обработать запрос и сформировать строку ответа
		Sendbody,	//Запрос обработан, идет отправка ответа
		CGI,		//Это cgi, нужно обработать по другому
		Closefd,	//Нужно закрыть fd, удалить из соответвтвующих мест
		AutoIndex	//хз
	};

	typedef	struct	s_dataFd
	{
		e_statusSession					statusFd;
		const ConfigServer				*configServer;
		std::string						requestHead;
		std::string						requestBody;
		std::string						responseHead;
		std::string						responseBody;
		size_t							sendBodyByte;
		HttpRequest 					httpRequest;
	}				t_dataFd;

	typedef struct	s_dataResp
	{
		std::map<int, t_dataFd *>		dataFd;
	}				t_dataResp;

	class Responder
	{
		private:

			void	_makeSession(int &fd, t_dataResp &data);
			void	_readBody(int &fd, t_dataResp &data);
			void	_send(int &fd, t_dataResp &data);
			void	_sendBody(int &fd, t_dataResp &data);
			void	_cgi(int &fd, t_dataResp &data);
			void	_closeFd(int &fd, t_dataResp &data);
			void	_autoIndex(int &fd, t_dataResp &data);
			void	_setStatusRequest(t_dataFd *data);

		public:
			Responder();
			virtual	~Responder();

			void	action(int &fd, t_dataResp &data);
	};

}
