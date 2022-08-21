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
		Location &operator=(const Location &other);
		virtual ~Location();

		void setUrl(const std::string &url);
		const std::string &getUrl() const;

		void setBinPathPy(const std::string &path);
		const std::string &getBinPathPy() const;

		void setBinPathSh(const std::string &path);
		const std::string &getBinPathSh() const;

		void setIsFolder(const bool &status);
		const bool &getIsFolder() const;

		void setIsRedirect(const bool &status);
		const bool &getIsRedirect() const;

		void setIsCgi(const bool &status);
		const bool &getIsCgi() const;

		void setRedirectionCode(const int &redirectionCode);
		const int &getRedirectionCode() const;
	};

}
