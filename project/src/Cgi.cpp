#include "../inc/Cgi.hpp"

ft::Cgi::Cgi()
{
	_hasChildProcess = false;
	_isSh = false;
	_isPy = false;
	_outFd = -1;
	_inFd = -1;
	_pid = -1;
}

ft::Cgi::~Cgi()
{
	//free cmd
	for (size_t i = 0; _cmd[i] != NULL; ++i)
		free(_cmd[i]);
	//need kill fork
	if (_pid > 0)
		kill(_pid, SIGTERM);
	//delete create file: _outName, _inName
	if (_outFd > 0)
		close(_outFd);
	if (_inFd > 0)
		close(_inFd);
}

char	ft::Cgi::parseURL(DataFd &data)
{
	std::string	fullURL;
	char		buf[2048];
	size_t		posStart;
	size_t		posEnd;

	fullURL = data.httpRequest->getUrl();
	_rootPath = data.loc->getRoot();
	getcwd(buf, 2048);
	_pathTranslated = std::string(buf) + "/www" + data.configServer->getRoot() + _rootPath;
	_outName = _pathTranslated + "/outCgi" + ft::Utils::to_string(data.fd);
	_inName = _pathTranslated + "/inCgi" + ft::Utils::to_string(data.fd);

	posStart = fullURL.find(data.loc->getUrl());
	if (posStart == std::string::npos)
		return (-1);
	posStart += data.loc->getUrl().length();
	posStart--;
	posEnd = fullURL.find('/', posStart + 1);
	if (posEnd == std::string::npos)
		_scriptName = fullURL.substr(posStart, fullURL.length() - posStart);
	else
		_scriptName = fullURL.substr(posStart, posEnd - posStart);
	_pathTranslated += _scriptName;
	if (posEnd == std::string::npos)
		return (0);
	posStart = posEnd;
	posEnd = fullURL.rfind('?');
	if (posEnd == std::string::npos)
	{
		_pathInfo = fullURL.substr(posStart, fullURL.length() - posStart);
		return (0);
	}
	else
		_pathInfo = fullURL.substr(posStart, posEnd - posStart);
	_queryString = fullURL.substr(posEnd + 1);
	return (0);
}

char	ft::Cgi::isCGI(DataFd &data)
{
	size_t		point;
	std::string	end;
	size_t		lenght;

	if (parseURL(data) < 0)
		return (0);
	point = _scriptName.rfind('.');
	lenght = _scriptName.length();
	if (point == std::string::npos || lenght <= 3)
		return (0);
	end = _scriptName.substr(point, _scriptName.length());
	if (end == ".py")
		_isPy = true;
	else if (end == ".sh")
		_isSh = true;
	else
	{
		data.code = ft::HTTP_PRECONDITION_FAILED;
		return (-1);
	}
	if (data.httpRequest->getMethod() != "POST" && \
	data.httpRequest->getMethod() != "GET" && \
	data.httpRequest->getMethod() != "HEAD")
	{
		data.code = ft::HTTP_METHOD_NOT_ALLOWED;
		return (-1);
	}
	return (1);
}

void	ft::Cgi::runChildProcess(DataFd &data)
{
	_pid = fork();
	if (fork() < 0)
	{
		data.code = ft::HTTP_INTERNAL_SERVER_ERROR;
		data.statusFd = ft::SendHead;
		return ;
	}
	if (_pid == 0)
		childProcess(data);
	_hasChildProcess = true;
}

void	ft::Cgi::formExecveData(DataFd &data)
{
	_cmd[0] = strdup((std::string("PATH_TRANSLATED=") + _pathTranslated).c_str());
	_cmd[1] = strdup((std::string("PATH_INFO=") + _pathInfo).c_str());
	_cmd[2] = strdup((std::string("QUERY_STRING=") + _queryString).c_str());
	_cmd[3] = strdup((std::string("REQUEST_METHOD=") + data.httpRequest->getMethod()).c_str());
	_cmd[4] = NULL;
}

void	ft::Cgi::childProcess(DataFd &data)
{
	if (data.httpRequest->getMethod() == "POST")
	{
		int	_inFd = open(_inName.c_str(), O_RDONLY | O_CREAT | O_TRUNC, 0666);
		dup2(_inFd, STDIN_FILENO);
		close(_inFd);
	}
	int	_outFd = open(_outName.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0666);
	dup2(_outFd, STDOUT_FILENO);
	close(_outFd);

	formExecveData(data);
	if (execve(_pathTranslated.c_str(), _cmd, NULL) == -1) {
		// perror("execve");
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}

void	ft::Cgi::waitChildProcess(DataFd &data)
{
	if (waitpid(_pid, NULL, WNOHANG) != 0)
	{
		_pid = -1;
		data.httpResponse->setBodyUrl(_outName);
		// data.httpResponse->setBodyType
		data.statusFd = ft::SendHead;
	}
	//if Script complete
	return ;
}

bool	ft::Cgi::hasChildProcess() const { return (_hasChildProcess); }

