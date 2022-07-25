#pragma once

#include "./ALocation.hpp"

namespace ft
{
	class Location : public ALocation
	{
		private:
			std::string _url;
			std::string _binPathPy;
			std::string _binPathSh;
			bool _isFolder;
			bool _isRedirect;
			bool _isCgi;
		public:
			Location();
			virtual ~Location();
	};

}
