#pragma once

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "./SimpleServer.hpp"

namespace FT {

#define BUFFSIZE 30000

class HTTPServer: public SimpleServer {
	private:
		char	buffer[BUFFSIZE];
		int		socketForClient;

		void	parseConfig();
		void	accepter();
		void	handler();
		void	responder();
	public:
		HTTPServer();
};

}

