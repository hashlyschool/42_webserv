
#pragma once

#include <stdexcept>
#include <iostream>
#include <istream>
#include <sstream>
#include <fstream>
#include <streambuf>
#include <iterator>
#include <string>
#include <vector>
#include <map>
#include "./ConfigServer.hpp"

#define default_config_file "./default_localhost.conf"

namespace ft
{
	class ConfigServer;

	enum Server_Conf
	{
		server_server_name,
		server_listen,
		server_root,
		server_index,
		server_autoindex,
		server_location,
		server_error_page,
		server_allowed_methods,
		server_max_body_size,
		server_upload_path,
		server_bin_path_py,
		server_bin_path_sh,
		server_redirection
	};

	typedef struct ServerBlock
	{
		std::map<std::string, std::vector<std::string> > server_param;
		std::map<int, std::string> server_err_page;
		std::vector<Location> server_locations;
	} serverBlockConfig_t;

	class Parser
	{
	public:
		typedef std::pair<std::string, std::vector<std::string> > dir_type;
		// typedef std::pair<std::string, std::string> dir_type;
		typedef std::pair<int, std::string> err_type;

		Parser(std::string pathConf);
		~Parser();
		bool read_file(const std::string &file_name, std::string &raw_record);
		void split_next_line(std::istringstream &input_stream, std::vector<std::string> &output_vector);

		// size_t getNumServers() const;
		const std::vector<ft::ConfigServer> &getConfigServers() const;

	private:
		// configuration data
		std::string _raw_config_file;
		std::string _pathConf;
		std::vector<ConfigServer> _servers;

		std::string _verify_input(int ac, char **av);
		void _parse_server_config(const std::string &conf_file);
		void _parse_server_block(std::istringstream &iss, int &brackets);
		int _check_name(const std::string &name);
		bool _server_param_check(const std::map<std::string, std::vector<std::string> > &param);
		void _erase_semicolon(std::vector<std::string> &parsed_line);
		void _erase_comma(std::vector<std::string> &parsed_line, size_t &i);
		void _checkDirective(std::vector<std::string> &parsed_line, serverBlockConfig_t &serverBlock);
		bool _validKeys(std::string &key, serverBlockConfig_t &serverBlock, size_t code);
		int _checkPortVal(std::string str);
		bool _isHost(std::string value);
		std::vector<std::string> _getHostPort(std::string host);
		err_type _fillErrorPage(std::vector<std::string> value);
		void _addServer(serverBlockConfig_t &serverBlock);
		void _setServerInfo(ssize_t index, serverBlockConfig_t &serverBlock);
		Parser();

		void _fillHostPort(std::vector<std::string>, ssize_t index);
		void _fillServerName(std::vector<std::string>, ssize_t index);
		void _fillAutoindex(std::vector<std::string>, ssize_t index);
		void _fillServerRoot(std::vector<std::string>, ssize_t index);
		void _fillIndex(std::vector<std::string>, ssize_t index);
		void _fillRootMethods(std::vector<std::string>, ssize_t index);
		void _fillRootMaxBodySize(std::vector<std::string>, ssize_t index);
		void _fillRootErrorPages(err_type value, ssize_t index);
		void _fillUploadPath(std::vector<std::string>, ssize_t index);

		void _fillLocationName(ft::Location &obj, std::string line);
		void _fillLocation(ft::Location &obj, std::string key, std::vector<std::string> args);
		void _fillLocationMethods(std::vector<std::string> args, ft::Location &location);
		void _fillLocationRoot(std::vector<std::string> args, ft::Location &location);
		void _fillLocationRedirection(std::vector<std::string> args, ft::Location &location);
		void _fillLocationErrorsPages(std::vector<std::string> args, ft::Location &location);
		void _fillLocationIndex(std::vector<std::string> args, ft::Location &location);
		void _fillLocationAutoindex(std::vector<std::string> args, ft::Location &location);
		void _fillLocationUploadPath(std::vector<std::string> args, ft::Location &location);
		void _fillLocationBinPathPy(std::vector<std::string> args, ft::Location &location);
		void _fillLocationBinPathSh(std::vector<std::string> args, ft::Location &location);
		void _fillConfig(ssize_t index, size_t key, std::vector<std::string>);
	};

	class ParserException : public std::exception
	{
		const std::string _text;

		ParserException();

	public:
		ParserException(const std::string &error_message) : _text(error_message){};
		~ParserException() throw(){};
		const char *what() const throw() { return _text.c_str(); };
	};
}
