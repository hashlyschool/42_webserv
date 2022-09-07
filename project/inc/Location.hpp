#pragma once

#include <map>
#include "./ALocation.hpp"

namespace ft
{
	class Location : public ALocation
	{
	private:
		std::string _binPathPy;
		std::string _binPathSh;

	public:
		Location();
		Location &operator=(const Location &other);
		virtual ~Location();

		void setBinPathPy(const std::string &path);
		const std::string &getBinPathPy() const;

		void setBinPathSh(const std::string &path);
		const std::string &getBinPathSh() const;
	};

}
