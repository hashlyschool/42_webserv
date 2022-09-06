#include "../inc/Cgi.hpp"

ft::Cgi::Cgi()
{
	_hasChildProcess = false;
	_outFd = -1;
	_inFd = -1;
	_cmd[0] = NULL;
	_cmd[1] = NULL;
}

ft::Cgi::~Cgi()
{
	//free cmd
	for (size_t i = 0; _cmd[i] != NULL; ++i)
		free(_cmd[i]);
	//need kill fork

	//delete create file: _outName, _inName
}

void	ft::Cgi::parseUrl()
{

}

void	ft::Cgi::waitChildProcess()
{
	return ;
}

void	ft::Cgi::runChildProcess()
{
	// data.cgi.parseQueryString();
	// data.cgi.preparseExecveData();
	_pid = fork();
	if (fork() < 0)
		cgi.error();
	if (pid == 0)
		cgi.childProcess();
	else
		cgi.parentProcess(pid);


	// data.dataFd[fd]->responseHead = cgi.getResponseHead();
	// data.dataFd[fd]->responseBody = cgi.getResponseBody();
	data.statusFd = ft::SendHead;

	//Попробовать подождать процесс дочерний
	//
	//createHead
	// data.dataFd[fd]->responseHead = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 22\n\n";
	//createBody
	// char	buf[2048];
	// bzero(buf, 2048);
	// read()
	// data.dataFd[fd]->responseBody = read
	// if (connection == keepAlive && time < MaxTime && Qrequests < max)
	// 	data.dataFd[fd]->statusFd = ft::Nosession;
	// else //close
	// 	data.dataFd[fd]->statusFd = ft::Closefd;
}

void	ft::Cgi::parseQueryString()
{
	// std::cout << "Parse query string" << std::endl;l
	_pathInfo = "test.py";
	_rootPath = "project/";
	_pathtranslated = "/home/ilya/School/git/MyWebServ";
	_outName = _pathtranslated + "/outCgi_test"; // + fd
	_pathtranslated += "/" + _rootPath + _pathInfo;
}

void	ft::Cgi::preparseExecveData()
{
	_cmd[0] = strdup(_pathtranslated.c_str());
	_cmd[1] = NULL;
}

void	ft::Cgi::childProcess()
{
	int	_outFd	= open(_outName.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0666);

	dup2(_outFd, STDOUT_FILENO);
	close(_outFd);
	if (execve(_cmd[0], _cmd, NULL) == -1) {
		// perror("execve");
		exit(EXIT_FAILURE);
	}
}

void	ft::Cgi::parentProcess(pid_t &pid)
{
	waitpid(pid, NULL, WNOHANG);
}

bool	ft::Cgi::isCGI(DataFd &data) const
{
	std::string url = data.httpRequest->getUrl();

	url.find(data.loc->getUrl(), url.begin());
	for (size_t i = 0; i < count; i++)
	{
		/* code */
	}

}

bool	ft::Cgi::hasChildProcess() const { return (_hasChildProcess); }

