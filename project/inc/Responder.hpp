#pragma once

#include <unistd.h>
#include <string>
#include <map>
#include <sys/socket.h>
#include <cstdlib>
#include <sys/time.h>

#include "./ConfigServer.hpp"
#include "./HttpRequest.hpp"
#include "./HttpResponse.hpp"
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
		Execute,	// исполнение запроса
		SendHead,		//Чтение закончено, нужно обработать запрос и сформировать строку ответа
		Sendbody,	//Запрос обработан, идет отправка ответа
		CGI,		//Это cgi, нужно обработать по другому
		Closefd,	//Нужно закрыть fd, удалить из соответвтвующих мест
		AutoIndex	//хз
	};

	typedef	struct	s_dataFd
	{
		size_t							code;
		std::string						finalUrl;
		e_statusSession					statusFd;
		const ConfigServer				*configServer;
		HttpRequest 					httpRequest;
		HttpResponse					httpResponse;
		Cgi								cgi;
		struct timeval					timeLastAction;
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
			void	_execute(int &fd, t_dataResp &data);
			void	_sendHead(int &fd, t_dataResp &data);
			void	_sendBody(int &fd, t_dataResp &data);
			void	_cgi(int &fd, t_dataResp &data);
			void	_closeFd(int &fd, t_dataResp &data);
			void	_autoIndex(int &fd, t_dataResp &data);
			void	_setStatusRequest(t_dataFd *data);

			void	_get(t_dataFd *data);
			void	_post(t_dataFd *data);
			void	_delete(t_dataFd *data);
		public:
			Responder();
			virtual	~Responder();

			void	action(int &fd, t_dataResp &data);
	};

}
