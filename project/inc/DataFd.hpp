#pragma once

#include <string>
#include <sys/time.h>

#include "./HttpRequest.hpp"
#include "./HttpResponse.hpp"
#include "./Cgi.hpp"
#include "./ConfigServer.hpp"
// #include "./Responder.hpp"

namespace ft
{
	class HttpResponse;
	class HttpRequest;
	class Cgi;

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

	class	DataFd
	{
		private:
			struct timeval		_timeLastAction;

		public:
			DataFd(int fd);
			~DataFd();

			int					fd;
			size_t				code;
			std::string			finalUrl;
			e_statusSession		statusFd;
			const ConfigServer	*configServer;
			const ALocation		*loc;
			HttpRequest 		*httpRequest;
			HttpResponse		*httpResponse;
			Cgi					*cgi;
			std::ofstream		*outFile;

			/* methods */
			void				updateTime();
			void				clear();

			/* getters */
			struct timeval		getTimeLastAct() const;
	};

	typedef std::map<int, DataFd *>			MapDataFd;
}
