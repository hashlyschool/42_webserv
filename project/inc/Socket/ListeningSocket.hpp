#pragma once

#include <stdio.h>
#include "./BindingSocket.hpp"

namespace FT {

	// Class for binding socket
	class ListeningSocket: public BindingSocket {
		private:
			int backlog;
			int start_listening();

		public:
			// Constructor
			ListeningSocket(int domain, int service, int protocol, int port, u_long interface, int backlog);
	};
}

