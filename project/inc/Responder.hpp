#pragma once

#include <unistd.h>
#include <string>
#include <map>
#include <sys/socket.h>
#include <cstdlib>
#include <sys/time.h>

// #include "./ConfigServer.hpp"
// #include "./HttpRequest.hpp"
// #include "./HttpResponse.hpp"
// #include "./Cgi.hpp"
#include "./DataFd.hpp"
#include "./Utils.hpp"

namespace ft
{
	class Responder
	{
		private:

			void	_makeSession(int &fd, DataFd &data);
			void	_readBody(int &fd, DataFd &data);
			void	_execute(DataFd &data);
			void	_sendHead(int &fd, DataFd &data);
			void	_sendBody(int &fd, DataFd &data);
			void	_cgi(DataFd &data);
			void	_closeFd(int &fd, MapDataFd &data);
			void	_autoIndex(int &fd, MapDataFd &data);

			void	_setStatusRequest(DataFd *data);

			void	_get(DataFd *data);
			void	_post(DataFd *data);
			void	_delete(DataFd *data);

			bool	_fileGoodForPost(DataFd *data);


		public:
			Responder();
			virtual	~Responder();

			void	action(int &fd, MapDataFd &data);
	};

}
