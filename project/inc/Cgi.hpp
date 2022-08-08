#pragma once

#include <iostream>
#include <string>
#include <string.h>
#include <unistd.h>
//open
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
//waitpid
#include <sys/wait.h>
//perror
#include <cstdlib>
//ifstream
#include <fstream>

namespace ft
{

	class Cgi
	{
		private:
			std::string	_pathtranslated; //absolute Path to scripts
			std::string	_rootPath;
			std::string	_pathInfo;
			std::string	_outName;
			std::string	_inName;
			int			_outFd;
			int			_inFd;
			char		*_cmd[2];

		public:
			Cgi();
			~Cgi();

			void			parseQueryString();
			void			preparseExecveData();
			void			childProcess();
			void			ParentProcess(pid_t &pid);
			void			error();
			std::string		getResponseHead();
			std::string		getResponseBody();
	};

}

