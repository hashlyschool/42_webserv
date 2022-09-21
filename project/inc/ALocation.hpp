#pragma once

#ifndef ALOCATION_HPP
#define ALOCATION_HPP

#include <string>
#include <iostream>
#include <iterator>
#include <map>

namespace ft
{
	class ALocation
	{
	protected:
		ALocation();
		ALocation &operator=(const ALocation &other);

		std::string					_root;
		std::string					_index;
		std::string					_uploadPath;
		std::string					_url;

		bool						_isGet;
		bool						_isPost;
		bool						_isDelete;
		bool						_isCgi;
		bool						_isRedirect;
		bool						_autoIndex;

		int							_redirectionCode;
		std::string					_redirectonUri;

		std::map<int, std::string>	_errorPages;

	public:
		virtual ~ALocation();

		void setRoot(const std::string &root);
		const std::string &getRoot() const;

		void setIndex(const std::string &index);
		const std::string &getIndex() const;

		void setUploadPath(const std::string &path);
		const std::string &getUploadPath() const;

		void setIsGet(const bool &status);
		const bool &getIsGet() const;

		void setIsPost(const bool &status);
		const bool &getIsPost() const;

		void setIsDelete(const bool &status);
		const bool &getIsDelete() const;

		void setIsCgi(const bool &status);
		const bool &getIsCgi() const;

		void setAutoIndex(const bool &autoIndex);
		const bool &getAutoIndex() const;

		void setUrl(const std::string &url);
		const std::string &getUrl() const;

		void setIsRedirect(const bool &status);
		const bool &getIsRedirect() const;

		void setRedirectionCode(const int &redirectionCode);
		const int &getRedirectionCode() const;

		void setErrorPages(const int &code, const std::string &path);
		const std::map<int, std::string> &getErrorPages() const;
	};
}

#endif
