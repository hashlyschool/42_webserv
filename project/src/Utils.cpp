#include "../inc/Utils.hpp"

unsigned long ft::Utils::strtoul(std::string string, int base)
{
	char *startString = &(string[0]);
	char *endString = &(string[string.length()]);

	return (std::strtoul(startString, &endString, base));
}

void ft::Utils::replaceAll(std::string &src, std::string toReplace, std::string replacement)
{
	size_t startPos = src.find(toReplace);
	size_t replacementLength = toReplace.length();

	while (startPos != std::string::npos)
	{
		src = src.replace(startPos, replacementLength, replacement);
		startPos = src.find(toReplace);
	}
}

std::string ft::Utils::readFromSocket(int fd, int buf_size)
{
	char		*buf = new char[buf_size + 1];
	int			status;
	std::string	ret;

	status = recv(fd, buf, buf_size, 0);
	if (status < 0)
	{
		delete [] buf;
		std::exit(-1);
	}
	buf[status] = 0;
	ret = buf;
	delete [] buf;
	return ret;
}

size_t ft::Utils::getdelim(std::string source, std::string &buffer, std::string delimeter, size_t pos)
{
	size_t startDelim = source.find(delimeter, pos);
	if (startDelim == std::string::npos)
	{
		buffer = source.substr(pos); // get the remainder anyway
		return startDelim;
	}
	buffer = source.substr(pos, startDelim - pos);
	return startDelim + delimeter.length();
}

void	ft::Utils::signalHandler(int signal)
{
	if (signal)
		signal = 0;
	std::cout <<	"--------[INFO]--------\n" << \
					"To turn off the web server, type: \'EXIT\' or \'Ctrl+D\'\n" << \
					"--------[INFO]--------\n";
}

void	ft::Utils::checkArg(int argc, char **argv)
{
	if (argc != 2)
		throw std::runtime_error("[ERROR] for start webserv using: ./webserv path_conf");
	//need open config, check chmod
	if (!argv)
		throw std::runtime_error("[ERROR] error open config file");
}

void	ft::Utils::setSignal()
{
	if (std::signal(SIGPIPE, SIG_IGN) == SIG_ERR)
		throw std::runtime_error("error set handler for SIGPIPE");
	if (std::signal(SIGINT, ft::Utils::signalHandler))
		throw std::runtime_error("error set handler for SIGINT");
	if (std::signal(SIGQUIT, ft::Utils::signalHandler))
		throw std::runtime_error("error set handler for SIGQUIT");
}

unsigned long  ft::Utils::getFileSize(std::string path)
{
	struct stat64 stat_buf;
    int rc = stat64(path.c_str(), &stat_buf);
    return stat_buf.st_size;
}
