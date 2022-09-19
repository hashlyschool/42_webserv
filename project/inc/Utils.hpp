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
#include <string.h>
#include <stdio.h>
#include <ctime>

namespace ft
{
	#define DELIMITER	"\r\n\r\n"
	#define HEADERS_DELIMITER "\r\n"
	#define BUF_SIZE 2048

	class Utils
	{
		private:
		static std::string		_getAutoIndexTable(const std::string &path, const std::string &reqUrl);

		public:
		static unsigned long	strtoul(std::string string, int base);
		static void				replaceAll(std::string &src, std::string toReplace, std::string replacement);
		static ssize_t			readFromSocket(int fd, char *buf, int buf_size);
		static size_t			getdelim(std::string source, std::string &buffer, std::string delimeter, size_t pos = 0);
		static void				signalHandler(int signal);
		static void				checkArg(int argc, char **argv);
		static void				setSignal();
		static unsigned long	getFileSize(std::string path);
		static std::string		getFileSizeNormalized(std::string path);
		static int				findMaxElem(std::list<int> list);
		static char				*getEnvStr(std::string key, std::string value);
		static std::string				&normalizedPath(std::string &path);

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
		static std::string		fileTimeModified(std::string url);

		static std::string		generateAutoIndex(const std::string &path, const std::string &reqUrl);
	};
}
