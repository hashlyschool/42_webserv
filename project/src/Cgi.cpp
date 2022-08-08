#include "../inc/Cgi.hpp"

ft::Cgi::Cgi() {}

ft::Cgi::~Cgi()
{
	//free cmd
	for (size_t i = 0; _cmd[i] != NULL; ++i)
		free(_cmd[i]);
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

void	ft::Cgi::ParentProcess(pid_t &pid)
{
	waitpid(pid, NULL, WNOHANG);
}

void	ft::Cgi::error()
{
	std::cout << "[ERROR]	CGI" << std::endl;
}

std::string	ft::Cgi::getResponseHead()
{
	std::string	ret;
	ret = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 22\n\n";
	return (ret);
}

std::string	ft::Cgi::getResponseBody()
{
	// std::ifstream	output(_outName.c_str(), std::ios::binary);

	// if (!output.good())
	// {
	// 	//code = 500;
	// 	throw std::runtime_error("cgi, ifstream");
	// }
	return (_outName);
}
