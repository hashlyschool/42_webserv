#pragma once

#include <unistd.h>
#include <string>

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
