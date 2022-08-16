
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

namespace ft
{

#define default_config_file "./default_localhost.conf"
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
		server_allowed_methods
	};

	typedef struct Location
	{
		std::string location_pathname;
		std::map<std::string, std::vector<std::string> > location_directives;
	} location_t;

	typedef struct ServerBlock
	{
		std::map<std::string, std::string> server_param;
		std::map<int, std::string> server_err_page;
		std::vector<Location> server_locations;
	} serverBlockConfig_t;

	class Parser
	{
	private:
		std::string _verify_input(int ac, char **av);
		void _parse_server_config(const std::string &conf_file);
		void _parse_server_block(std::istringstream &iss, std::vector<std::string> &parsed_line, serverBlockConfig_t &serverBlock, int &brackets);
		int _check_name(const std::string &name);
		bool _server_param_check(const std::map<std::string, std::string> &param);
		Parser();

	protected:
		// configuration data
		std::string raw_config_file;
		std::string _pathConf;
		std::vector<ConfigServer *> server_group;

		void AddServer(serverBlockConfig_t &serverBlock);

	public:
		Parser(std::string pathConf);
		~Parser();
		void parse_config();
		bool read_file(const std::string &file_name, std::string &raw_record);
		void split_next_line(std::istringstream &input_stream, std::vector<std::string> &output_vector);

		size_t getNumServers() const;
		ConfigServer *getConfigServer(int num) const;
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
