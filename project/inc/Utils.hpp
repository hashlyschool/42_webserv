#pragma once

#include <string>
#include <cstdlib>
#include <sys/socket.h>
#include <iostream>
#include <csignal>
#include <sstream>
#include <sys/stat.h>
#include <list>
#include <dirent.h>
#include <unistd.h>

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
		static int				findMaxElem(std::list<int> list);

		template <typename T>
		static std::string		to_string(T elem)
		{
			std::stringstream sstream;
			sstream << elem;
			return sstream.str();
		}

		static bool				fileExists(std::string url);
		static bool				pathToFileExists(std::string url);
		static bool				fileIsReadable(std::string url);
		static bool				fileIsWritable(std::string url);
		static bool				isDirectory(std::string url);
		static bool				isNotEmptyDirectory(std::string url);
		static bool				pathToFileIsWritable(std::string url);

	};
}
