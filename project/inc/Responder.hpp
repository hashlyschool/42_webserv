#pragma once

#include <unistd.h>
#include <string>
#include <map>
#include <sys/socket.h>
#include "./ConfigServer.hpp"

namespace ft
{

	enum e_statusSession
	{
		Nosession,
		Readbody,
		Send,
		Sendbody,
		Cgi,
		Closefd,
		AutoIndex
	};

	typedef	struct	s_dataFd
	{
		e_statusSession				statusFd;
	}				t_dataFd;

	typedef struct s_dataResp
	{
		std::vector<ConfigServer*>	configServers;
		std::map<int, t_dataFd *>	dataFd;
	}	t_dataResp;

	class Responder
	{
		private:

		public:
			Responder();
			virtual	~Responder();

			e_statusSession	action(int &fd, t_dataResp &data);
	};

}
