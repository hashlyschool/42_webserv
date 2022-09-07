#include "../inc/Location.hpp"

ft::Location::Location()
{
	_url = "";
	_binPathPy = "";
	_binPathSh = "";
	_isRedirect = false;
	_isCgi = false;
}

ft::Location::~Location()
{
}

ft::Location &ft::Location::operator=(const Location &other)
{
	ALocation::operator=(other);
	_url = other._url;
	_binPathPy = other._binPathPy;
	_binPathSh = other._binPathSh;
	_isRedirect = other._isRedirect;
	_isCgi = other._isCgi;
	_errorPages.insert(other._errorPages.begin(), other._errorPages.end());
	return *this;
}

void ft::Location::setBinPathPy(const std::string &path)
{
	_binPathPy = path;
}

const std::string &ft::Location::getBinPathPy() const
{
	return _binPathPy;
}

void ft::Location::setBinPathSh(const std::string &path)
{
	_binPathSh = path;
}

const std::string &ft::Location::getBinPathSh() const
{
	return _binPathSh;
}
