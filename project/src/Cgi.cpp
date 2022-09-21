#include "../inc/Cgi.hpp"

ft::Cgi::Cgi()
{
	_isPy = false;
	_isSh = false;
	_isCGI = -2;
	_pathInterpreter = NULL;
	_hasChildProcess = false;

	_pid = -1;
	_outFd = -1;
	_inFd = -1;
	for (size_t i = 0; i < SIZE_ARGV; ++i)
		_argv[i] = NULL;
	for (size_t i = 0; i < SIZE_ENV; ++i)
		_env[i] = NULL;
}

ft::Cgi::~Cgi()
{
	for (size_t i = 0; _argv[i] != NULL; ++i)
		free(_argv[i]);
	for (size_t i = 0; _env[i] != NULL; ++i)
		free(_env[i]);
	if (_pid > 0)
		kill(_pid, SIGTERM);
	if (_outFd > 0)
	{
		remove(_outName.c_str());
		close(_outFd);
	}
	if (_inFd > 0)
	{
		close(_inFd);
		remove(_inName.c_str());
	}
}

char	ft::Cgi::parseURL(DataFd &data)
{
	std::string	fullURL;
	size_t		posStart;
	size_t		posEnd;

	fullURL = data.httpRequest->getUrl();
	_rootPath = data.loc->getRoot();
	_outName = _rootPath + "/outCgi" + ft::Utils::to_string(data.fd);
	_inName = _rootPath + "/inCgi" + ft::Utils::to_string(data.fd);
	posStart = fullURL.find(data.loc->getUrl());
	if (posStart == std::string::npos)
		return (-1);
	posStart += data.loc->getUrl().length();
	posEnd = fullURL.find('/', posStart + 1);
	if (posEnd == std::string::npos)
		posEnd = fullURL.find('?', posStart + 1);
	if (posEnd == std::string::npos)
		_scriptName = fullURL.substr(posStart, fullURL.length() - posStart);
	else
		_scriptName = fullURL.substr(posStart, posEnd - posStart);
	_pathTranslated = _rootPath + data.loc->getUrl();
	_pathTranslated += _scriptName;
	if (posEnd == std::string::npos)
		return (0);
	posStart = posEnd;
	posEnd = fullURL.rfind('?');
	if (posEnd == std::string::npos)
	{
		_pathInfo = fullURL.substr(posStart, fullURL.length() - posStart);
		ft::Utils::normalizedPath(_pathInfo);
		return (0);
	}
	else
	{
		_pathInfo = fullURL.substr(posStart, posEnd - posStart);
		ft::Utils::normalizedPath(_pathInfo);
	}
	_queryString = fullURL.substr(posEnd + 1);
	return (0);
}

char	ft::Cgi::isCGI(DataFd &data)
{
	size_t		point;
	std::string	end;
	size_t		lenght;

	if (_isCGI > -2)
		return (_isCGI);
	if (data.loc == NULL || parseURL(data) < 0)
	{
		_isCGI = 0;
		return (_isCGI);
	}
	point = _scriptName.rfind('.');
	lenght = _scriptName.length();
	if (point == std::string::npos || lenght <= 3)
	{
		_isCGI = 0;
		return (_isCGI);
	}
	end = _scriptName.substr(point, _scriptName.length());
	if (end == ".py")
		_isPy = true;
	else if (end == ".sh")
		_isSh = true;
	else
	{
		data.code = ft::HTTP_PRECONDITION_FAILED;
		_isCGI = -1;
		return (_isCGI);
	}
	if (data.httpRequest->getMethod() != "POST" && \
	data.httpRequest->getMethod() != "GET" && \
	data.httpRequest->getMethod() != "HEAD")
	{
		data.code = ft::HTTP_METHOD_NOT_ALLOWED;
		_isCGI = -1;
		return (_isCGI);
	}
	_isCGI = 1;
	return (_isCGI);
}

void	ft::Cgi::setPathInterpreter(DataFd &data)
{
	const Location *loc = dynamic_cast<const Location*>(data.loc);

	if (_isPy)
		_pathInterpreter = loc->getBinPathPy().c_str();
	else if (_isSh)
		_pathInterpreter = loc->getBinPathSh().c_str();
	else
		_pathInterpreter = NULL;
}

void	ft::Cgi::fillArgv()
{
	_argv[0] = strdup(_pathInterpreter);
	_argv[1] = strdup(_pathTranslated.c_str());
}

void	ft::Cgi::fillEnv(DataFd &data)
{
	_env[0] = ft::Utils::getEnvStr("AUTH_TYPE", "");//?
	_env[1] = ft::Utils::getEnvStr("CONTENT_LENGTH",ft::Utils::to_string(data.httpRequest->getContentLength()));
	_env[2] = ft::Utils::getEnvStr("CONTENT_TYPE", data.httpRequest->getContenType());
	_env[3] = ft::Utils::getEnvStr("DOCUMENT_ROOT", data.loc->getRoot()); //?
	_env[4] = ft::Utils::getEnvStr("GATEWAY_INTERFACE", "CGI/1.1");
	_env[5] = ft::Utils::getEnvStr("PATH_INFO", _pathInfo);
	_env[6] = ft::Utils::getEnvStr("PATH_TRANSLATED", _pathTranslated);
	_env[7] = ft::Utils::getEnvStr("QUERY_STRING", _queryString);
	_env[8] = ft::Utils::getEnvStr("REMOTE_ADDR", ""); //?
	_env[9] = ft::Utils::getEnvStr("REMOTE_HOST", ""); //?
	_env[10] = ft::Utils::getEnvStr("REMOTE_USER", ""); //?
	_env[11] = ft::Utils::getEnvStr("REQUEST_METHOD", data.httpRequest->getMethod());
	_env[12] = ft::Utils::getEnvStr("SCRIPT_NAME", _scriptName);
	_env[13] = ft::Utils::getEnvStr("SERVER_NAME", data.configServer->getServerName());
	_env[14] = ft::Utils::getEnvStr("SERVER_PORT", ""); //data.configServer->getPort() return u_short
	_env[15] = ft::Utils::getEnvStr("SERVER_PROTOCOL", data.httpRequest->getHttpVersion());
	_env[16] = ft::Utils::getEnvStr("SERVER_SOFTWARE", "webserv_21");

	_env[17] = ft::Utils::getEnvStr("HTTP_ACCEPT", ""); //?
	_env[18] = ft::Utils::getEnvStr("HTTP_FROM", ""); //?
	_env[19] = ft::Utils::getEnvStr("HTTP_REFERER", ""); //?
	_env[20] = ft::Utils::getEnvStr("HTTP_USER_AGENT", ""); //browser
}

void	ft::Cgi::createInOutFile(DataFd &data)
{
	if (data.httpRequest->getMethod() == "POST")
		_inFd = open(_inName.c_str(), O_RDONLY | O_CREAT | O_TRUNC, 0666);
	_outFd = open(_outName.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0666);
}

void	ft::Cgi::formExecveData(DataFd &data)
{
	createInOutFile(data);
	setPathInterpreter(data);
	fillArgv();
	fillEnv(data);
}

void	ft::Cgi::runChildProcess(DataFd &data)
{
	data.cgi->formExecveData(data);
	_pid = fork();
	if (_pid < 0)
	{
		data.code = ft::HTTP_INTERNAL_SERVER_ERROR;
		data.statusFd = ft::SendHead;
		return ;
	}
	if (_pid == 0)
		childProcess();
	_hasChildProcess = true;
}

void	ft::Cgi::childProcess()
{
	if (_inFd > 0)
		dup2(_inFd, STDIN_FILENO);
	dup2(_outFd, STDOUT_FILENO);
	dup2(_outFd, STDERR_FILENO);
	if (execve(_pathInterpreter, _argv, _env) == -1) {
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}

size_t	ft::Cgi::parseFirstLine(DataFd &data, std::string line)
{
	size_t			pos = 0;
	std::string		temp;
	int				code;

	pos = ft::Utils::getdelim(line, temp, " ", pos);
	if (pos == std::string::npos)
		return (pos);

	pos = ft::Utils::getdelim(line, temp, " ", pos);
	if (pos == std::string::npos)
		return (pos);

	code = ft::Utils::strtoul(temp, 10);
	if (code == 0)
		return (std::string::npos);

	pos = ft::Utils::getdelim(line, temp, "\n", pos);
	if (pos == std::string::npos)
		return (pos);

	data.code = code;
	return pos;
}

void	ft::Cgi::parseHeadLine(DataFd &data, std::string line)
{
	size_t	pos = 0;

	pos = line.find("Content-Type: ", 0);
	if (pos == std::string::npos)
		return ;
	data.httpResponse->setBodyType(line.substr(14));
}

int	ft::Cgi::parseOutFile(DataFd &data)
{
	std::string			temp;
	std::fstream		outFile;
	size_t				bytesRead;

	// if (_scriptName.find("nph-") == 0)
	// 	return ;
	outFile.open(_outName.c_str());
	if (outFile.fail() || outFile.bad())
		return (-1);
	std::getline(outFile, temp);
	if (parseFirstLine(data, temp) == std::string::npos)
		parseHeadLine(data, temp);
	for (; std::getline(outFile, temp); )
	{
		if (temp == "\n" || temp == "r\n" || temp == "")
			break ;
		parseHeadLine(data, temp);
		std::getline(outFile, temp);
	}
	bytesRead = outFile.tellp();
	data.httpResponse->setBytesRead(bytesRead);
	data.httpResponse->setBodySize(ft::Utils::getFileSize(_outName) - bytesRead);
	data.statusFd = ft::SendHead;
	outFile.close();
	return 1;
}

void	ft::Cgi::waitChildProcess(DataFd &data)
{
	if (waitpid(_pid, NULL, WNOHANG) == _pid)
	{
		_pid = -1;
		data.finalUrl = _outName;
		data.statusFd = ft::SendHead;
		_hasChildProcess = false;
		// if (_scriptName.find("nph-") == 0)
		// 	return ;
		// _isSh = false;
		// _isPy = false;
		// if (parseOutFile(data) < 0)
		// 	data.code = ft::HTTP_INTERNAL_SERVER_ERROR;
	}
	return ;
}

bool	ft::Cgi::hasChildProcess() const { return (_hasChildProcess); }

