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

ssize_t ft::Utils::readFromSocket(int fd, char *buf, int buf_size)
{
	ssize_t	status;

	status = recv(fd, buf, buf_size, 0);
	return status;
}

size_t ft::Utils::getdelim(std::string source, std::string &buffer, std::string delimeter, size_t pos)
{
	size_t startDelim = source.find(delimeter, pos);
	if (startDelim == std::string::npos)
	{
		if (pos != std::string::npos)
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
	std::cout << "--------[INFO]--------\n" << \
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

std::string ft::Utils::getFileSizeNormalized(std::string path)
{
	unsigned long size = getFileSize(path);
	std::string sizeType = "Bytes";
	if (size / 1024 > 0)
	{
		size /= 1024;
		sizeType = "KB";
		if (size / 1024 > 0)
		{
			size /= 1024;
			sizeType = "MB";
		}
	}
	return to_string(size) + " " + sizeType;
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

char	*ft::Utils::getEnvStr(std::string key, std::string value)
{
	return (strdup((key + "=" + value).c_str()));
}

std::string &ft::Utils::normalizedPath(std::string &path)
{
	if (!path.empty())
	{
		if (path[0] != '/')
			path = "/" + path;
		if (path[path.size() - 1] == '/')
			path.erase(path.size() - 1, 1);
	}
	return (path);
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

std::string ft::Utils::fileTimeModified(std::string url) {
	struct stat stat_buf;
	struct tm *clock;

	if (stat(url.c_str(), &stat_buf) < 0)
		return "NOT DEFINED";

	clock = gmtime(&(stat_buf.st_mtim.tv_sec));
	return (asctime(clock));
}


std::string ft::Utils::_getAutoIndexTable(const std::string &path, const std::string &reqUrl)
{
	DIR  *dp;
	struct dirent *di_struct;
	int i = 0;
	std::stringstream table;

	dp = opendir(path.data());
	table << "<h1>" << reqUrl << "</h1>" <<
			"<table>" <<
			"<tr> <th>File name</th>"<<
			"<th> File size</th>" <<
			"<th>Last modified</th> </tr>";
	if (dp != NULL) {
		while ((di_struct = readdir(dp)) != NULL) {
			std::string pathFile(path + "/" + di_struct->d_name);
			table << "<tr>" <<
					"<td><a href=\"" << reqUrl + di_struct->d_name;
			if (isDirectory(pathFile))
				table << "/";
			table << "\">";
			table << std::string(di_struct->d_name);
			if (isDirectory(pathFile))
				table << "/";
			table << "</a></td>";
			table << "<td>" << getFileSizeNormalized(pathFile) << "</td>";
			table << "<td>" + fileTimeModified(pathFile) << "</td>";
			table << "</tr>";
			i++;
		}
		closedir(dp);
	}
	table << "</table>";
	return table.str();
}

std::string ft::Utils::generateAutoIndex(const std::string & path, const std::string & reqUrl)
{
	std::string table = Utils::_getAutoIndexTable(path, reqUrl);
	std::stringstream sstream;
	sstream << "<!DOCTYPE html><html lang=\"en\">" <<
				"<head><meta charset=\"UTF-8\">" <<
				"<title>" << reqUrl << "</title>" <<
				"</head>" <<
				"<style>" <<
				"table {" <<
				"border: 1px solid #ccc;" <<
				"background-color: #f8f8f8;" <<
				"border-collapse: collapse;" <<
				"margin: 0;" <<
				"padding: 0;" <<
				"width: 100%;" <<
				"table-layout: fixed;" <<
				"text-align: left;" <<
				"} table " <<
				"td:last-child " <<
				"{border-bottom: 0;}" <<
				"</style>" <<
				"<body>" << table << "</body></html>";
	return sstream.str();
}
