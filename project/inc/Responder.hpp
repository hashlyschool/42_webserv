#pragma once

#include <unistd.h>
#include <string>
#include <sys/socket.h>

namespace ft
{

	class Responder
	{
		private:

		public:
			Responder();
			~Responder();

			void	action(int fd);
	};

}
