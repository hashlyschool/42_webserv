#pragma once

#include <string>
#include <cstdlib>
#include <sys/socket.h>
#include <iostream>
#include <csignal>
#include <sstream>
#include <sys/stat.h>

namespace ft
{
	class Utils
	{
		public:
		static unsigned long	strtoul(std::string string, int base);
		static void				replaceAll(std::string &src, std::string toReplace, std::string replacement);
		static std::string		readFromSocket(int fd, int buf_size);
		static size_t			getdelim(std::string source, std::string &buffer, std::string delimeter, size_t pos = 0);
		static void				signalHandler(int signal);
		static void				checkArg(int argc, char **argv);
		static void				setSignal();
		static unsigned long	getFileSize(std::string path);

		template <typename T>
		static std::string		to_string(T elem)
		{
			std::stringstream sstream;
			sstream << elem;
			return sstream.str();
		}
	};
}
