#include "../inc/Server.hpp"

Server::Server(): _path_config("default_config") {}

Server::Server(std::string path_config): _path_config(path_config) {}

Server::~Server() {}
