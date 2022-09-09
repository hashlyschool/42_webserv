#pragma once

#include <iostream>
#include <string>
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
#include "./Utils.hpp"

namespace ft
{
	#define SIZE_ENV 22
	#define SIZE_ARGV 3

	class DataFd;

	class Cgi
	{
		private:
			std::string	_rootPath; //+
			std::string	_scriptName; //+
			std::string	_pathInfo; //+
			std::string	_queryString; //-
			std::string	_pathTranslated; //+
			// URL = /cgi-bin/script.py/tempFolser/pass?and=1&or=2
			// URL = [requestURL][ScriptName][PATH_INFO][QUERY_STRING]
			bool		_isPy; //+
			bool		_isSh; //+
			const char	*_pathInterpreter;
			bool		_hasChildProcess;

			pid_t		_pid;
			std::string	_outName; //+
			std::string	_inName; //+

			int			_outFd;
			int			_inFd;

			char		*_argv[SIZE_ARGV];
			char		*_env[SIZE_ENV];
			void		setPathInterpreter(DataFd &data);
			void		fillArgv();
			void		fillEnv(DataFd &data);
			void		createInOutFile(DataFd &data);

			char		parseURL(DataFd &data);
			void		formExecveData(DataFd &data);
			void		childProcess();
		public:
			Cgi();
			~Cgi();

			char	isCGI(DataFd &data);

			void	runChildProcess(DataFd &data);
			void	waitChildProcess(DataFd &data);

			/* getters */
			bool	hasChildProcess() const;
	};

}

