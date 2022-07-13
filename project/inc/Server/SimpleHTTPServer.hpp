#pragma once

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "SimpleServer.hpp"

namespace FT {

#define BUFFSIZE 30000

	class SimpleHTTPServer: public SimpleServer {
		private:
			char buffer[BUFFSIZE];
			int socketForClient;
			void accepter();
			void handler();
			void responder();
		public:
			SimpleHTTPServer();
	};
}

