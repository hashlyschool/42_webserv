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
	if (stat64(path.c_str(), &stat_buf) < 0)
		return 0;
	return stat_buf.st_size;
}

int	ft::Utils::findMaxElem(std::list<int> list)
{
	int	max;

	max = list.front();
	for (std::list<int>::iterator it = list.begin(); it != list.end(); ++it)
	{
		if (*it > max)
			max = *it;
	}
	return max;
}

bool ft::Utils::fileExists(std::string url)
{
	struct stat stat_buf;
	int res = stat(url.c_str(), &stat_buf);
	return (res == 0);
}

bool ft::Utils::pathToFileExists(std::string url)
{
	size_t pos = url.find_last_of('/');
	if (pos == std::string::npos) // not sure if it ever happens but just in case
		return false;
	std::string pathToDir = url.substr(0, pos);
	if (pathToDir.empty()) // not sure if it ever happens but just in case
		return false;
	if (isDirectory(pathToDir))
		return true;
	return false;
}

bool ft::Utils::pathToFileIsWritable(std::string url)
{
	std::string pathToDir = url.substr(0, url.find_last_of('/'));
	if (fileIsWritable(pathToDir))
		return true;
	return false;
}

bool ft::Utils::fileIsReadable(std::string url)
{
	struct stat stat_buf;
	stat(url.c_str(), &stat_buf);
	return (stat_buf.st_mode & S_IREAD);
}

bool ft::Utils::fileIsWritable(std::string url)
{
	struct stat stat_buf;
	stat(url.c_str(), &stat_buf);
	return (stat_buf.st_mode & S_IWRITE);
}

bool ft::Utils::isDirectory(std::string url)
{
	struct stat stat_buf;
	stat(url.c_str(), &stat_buf);
	return (stat_buf.st_mode & S_IFDIR);
}

bool ft::Utils::isNotEmptyDirectory(std::string url) {
  int n = 0;
  struct dirent *d;
  DIR *dir = opendir(url.c_str());
  if (dir == NULL) //Not a directory or doesn't exist
    return false;
  while ((d = readdir(dir)) != NULL) {
	n += 1;
	if (n > 2)
		break;
  }
  closedir(dir);
  if (n <= 2) //Directory Empty
    return false;
  else
    return true;
}

/*
std::string ft::Utils::normalizeUri(std::string s)
{
    std::list<std::string>                   tokens;
    std::list<std::string>::reverse_iterator tmp1;
    std::list<std::string>::reverse_iterator it = tokens.rbegin();
    int                                      i  = 0;

    if (s[0] != '/') {
        s.clear();
        return (s);
    }

    tokens = strTokenizer(s, '/');
    for (; it != tokens.rend(); ++it) {
        tmp1 = it;
        if (*it == "..")
            i += 4;
        if (i > 0) {
            --tmp1;
            tokens.erase(--(it.base()));
            it = tmp1;
            --i;
        }
    }
    if (i != 0) {
        s.clear();
        return (s);
    }
    s.clear();
    for (std::list<std::string>::iterator rit = tokens.begin(); rit != tokens.end(); ++rit) {
        if (*rit == "/") {
            if (++rit != tokens.end() && *(rit) == "/") {
                s.clear();
                return (s);
            } else
                --rit;
        }
        s.append(*rit);
    }
    return (s);
}
*/
