#pragma once

#include <string>
#include "./HttpRequest.hpp"
#include "./HttpResponse.hpp"
#include "./Cgi.hpp"
#include "ConfigServer.hpp"
#include <sys/time.h>
#include "./Responder.hpp"

namespace ft
{
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
	}			t_dataFd;
}
