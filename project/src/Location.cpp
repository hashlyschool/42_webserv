#include "../inc/Location.hpp"

ft::Location::Location()
{
	_url = "";
	_binPathPy = "";
	_binPathSh = "";
	_isFolder = false;
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
	_isFolder = other._isFolder;
	_isRedirect = other._isRedirect;
	_isCgi = other._isCgi;
	_errorPages.insert(other._errorPages.begin(), other._errorPages.end());
	return *this;
}

void ft::Location::setUrl(const std::string &url)
{
	_url = url;
}

const std::string &ft::Location::getUrl() const
{
	return _url;
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

void ft::Location::setIsFolder(const bool &status)
{
	_isFolder = status;
}

const bool &ft::Location::getIsFolder() const
{
	return _isFolder;
}

void ft::Location::setIsRedirect(const bool &status)
{
	_isRedirect = status;
}

const bool &ft::Location::getIsRedirect() const
{
	return _isRedirect;
}

void ft::Location::setIsCgi(const bool &status)
{
	_isCgi = status;
}

const bool &ft::Location::getIsCgi() const
{
	return _isCgi;
}
