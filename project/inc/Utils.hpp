#pragma once

#include <string>
#include <cstdlib>
#include <sys/socket.h>


namespace ft
{
	class Utils
	{
		public:
		static unsigned long	strtoul(std::string string, int base);
		static void				replaceAll(std::string &src, std::string toReplace, std::string replacement);
		static std::string		readFromSocket(int fd, int buf_size);
		static size_t			getdelim(std::string source, std::string &buffer, std::string delimeter, size_t pos = 0);
	};
}