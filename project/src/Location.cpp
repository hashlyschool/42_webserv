#include "../inc/Location.hpp"

ft::Location::Location()
{
	_url = "";
	_binPathPy = "";
	_binPathSh = "";
	_isFolder = 0;
	_isRedirect = 0;
	_isCgi = 0;
}

ft::Location::~Location()
{

}
