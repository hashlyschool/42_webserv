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

#include "./DataFd.hpp"

namespace ft
{

	class Cgi
	{
		private:
			std::string	_pathtranslated; //absolute Path to scripts
			std::string	_rootPath; //?
			 // /cgi-bin/script.py[PATH_INFO][QUERY_STRING]
			std::string	_pathInfo;
			std::string	_query_string;

			bool		_hasChildProcess;

			pid_t		_pid;
			std::string	_outName;
			std::string	_inName;

			int			_outFd;
			int			_inFd;

			char		*_cmd[2];

			void	parseUrl();
		public:
			Cgi();
			~Cgi();

			bool	isCGI(DataFd &data) const;


			void	runChildProcess();
			void	waitChildProcess();

			void	childProcess();
			void	parentProcess(pid_t &pid);


			/* getters */
			bool	hasChildProcess() const;
	};

}

