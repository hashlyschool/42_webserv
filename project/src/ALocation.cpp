#include "../inc/ALocation.hpp"

ft::ALocation::ALocation()
{
	_root = "";
	_index = "";
	_uploadPath = "";
	_isGet = false;
	_isPost = false;
	_isDelete = false;
	_autoIndex = false;
}

ft::ALocation &ft::ALocation::operator=(const ft::ALocation &other)
{
	_root = other._root;
	_index = other._index;
	_uploadPath = other._uploadPath;
	_isGet = other._isGet;
	_isPost = other._isPost;
	_isDelete = other._isDelete;
	_autoIndex = other._autoIndex;
	_errorPages = other._errorPages;
	return *this;
}

ft::ALocation::~ALocation()
{
}

void ft::ALocation::setRoot(const std::string &root)
{
	_root = root;
}

const std::string &ft::ALocation::getRoot() const
{
	return _root;
}

void ft::ALocation::setIndex(const std::string &index)
{
	_index = index;
}

const std::string &ft::ALocation::getIndex() const
{
	return _index;
}

void ft::ALocation::setUploadPath(const std::string &path)
{
	_uploadPath = path;
}

const std::string &ft::ALocation::getUploadPath() const
{
	return _uploadPath;
}

void ft::ALocation::setIsGet(const bool &status)
{
	_isGet = status;
}

const bool &ft::ALocation::getIsGet() const
{
	return _isGet;
}

void ft::ALocation::setIsPost(const bool &status)
{
	_isPost = status;
}

const bool &ft::ALocation::getIsPost() const
{
	return _isPost;
}

void ft::ALocation::setIsDelete(const bool &status)
{
	_isDelete = status;
}

const bool &ft::ALocation::getIsDelete() const
{
	return _isDelete;
}

void ft::ALocation::setAutoIndex(const bool &autoIndex)
{
	_autoIndex = autoIndex;
}

const bool &ft::ALocation::getAutoIndex() const
{
	return _autoIndex;
}

void ft::ALocation::setUrl(const std::string &url)
{
	_url = url;
}

const std::string &ft::ALocation::getUrl() const
{
	return _url;
}

void ft::ALocation::setErrorPages(const int &code, const std::string &path)
{
	_errorPages[code] = path;
}

std::map<int, std::string> &ft::ALocation::getErrorPages()
{
	return _errorPages;
}
