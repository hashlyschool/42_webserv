#include "../inc/HttpUtils.hpp"

const std::map<std::string, std::string> ft::HttpUtils::_mimeTypes = ft::HttpUtils::_initMimeTypes();

const std::map<std::string, std::string> ft::HttpUtils::_initMimeTypes()
{
	std::map<std::string, std::string> _mt;
	_mt["aac"] = "audio/aac";
	_mt["abw"] = "application/x-abiword";
	_mt["arc"] = "application/x-freearc";
	_mt["avif"] = "image/avif";
	_mt["avi"] = "video/x-msvideo";
	_mt["azw"] = "application/vnd.amazon.ebook";
	_mt["bin"] = "application/octet-stream";
	_mt["bmp"] = "image/bmp";
	_mt["bz"] = "application/x-bzip";
	_mt["bz2"] = "application/x-bzip2";
	_mt["cda"] = "application/x-cdf";
	_mt["csh"] = "application/x-csh";
	_mt["css"] = "text/css";
	_mt["csv"] = "text/csv";
	_mt["doc"] = "application/msword";
	_mt["docx"] = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
	_mt["eot"] = "application/vnd.ms-fontobject";
	_mt["gz"] = "application/gzip";
	_mt["gif"] = "image/gif";
	_mt["htm"] = "text/html";
	_mt["html"] = "text/html";
	_mt["ico"] = "image/vnd.microsoft.icon";
	_mt["ics"] = "text/calendar";
	_mt["jar"] = "application/java-archive";
	_mt["jpeg"] = "image/jpeg";
	_mt["jpg"] = "image/jpeg";
	_mt["js"] = "text/javascript";
	_mt["json"] = "application/json";
	_mt["jsonld"] = "application/ld+json";
	_mt["mid"] = "ausio/midi";
	_mt["midi"] = "ausio/midi";
	_mt["mjs"] = "text/javascript";
	_mt["mp3"] = "audio/mpeg";
	_mt["mp4"] = "audio/mp4";
	_mt["mpeg"] = "audio/mpeg";
	_mt["mpkg"] = "application/vnd.apple.installer+xml";
	_mt["odp"] = "application/vnd.oasis.opendocument.presentation";
	_mt["ods"] = "application/vnd.oasis.opendocument.spreadsheet";
	_mt["odt"] = "application/vnd.oasis.opendocument.text";
	_mt["oga"] = "audio/ogg";
	_mt["ogv"] = "video/ogg";
	_mt["ogx"] = "application/ogg";
	_mt["opus"] = "audio/opus";
	_mt["otf"] = "font/otf";
	_mt["png"] = "image/png";
	_mt["pdf"] = "application/pdf";
	_mt["php"] = "application/x-httpd-php";
	_mt["ppt"] = "application/vnd.ms-powerpoint";
	_mt["pptx"] = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
	_mt["rar"] = "application/vnd.rar";
	_mt["rtf"] = "application/rtf";
	_mt["sh"] = "application/x-sh";
	_mt["svg"] = "image/svg+xml";
	_mt["swf"] = "application/x-shockwave-flash";
	_mt["tar"] = "application/x-tar";
	_mt["tif"] = "image/tiff";
	_mt["tiff"] = "image/tiff";
	_mt["ts"] = "video/mp2t";
	_mt["ttf"] = "font/ttf";
	_mt["txt"] = "text/plain";
	_mt["vsd"] = "application/vnd.visio";
	_mt["wav"] = "audio/wav";
	_mt["weba"] = "audio/webm";
	_mt["webm"] = "video/webm";
	_mt["webp"] = "image/webp";
	_mt["woff"] = "font/woff";
	_mt["woff2"] = "font/woff2";
	_mt["xhtml"] = "application/xhtml+xml";
	_mt["xls"] = "application/vnd.ms-excel";
	_mt["xlsx"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
	_mt["xml"] = "application/xml";
	_mt["xul"] = "application/vnd.mozilla.xul+xml";
	_mt["zip"] = "application/zip";
	_mt["3gp"] = "video/3gpp";
	_mt["3g2"] = "video/3gpp2";
	_mt["7z"] = "application/x-7z-compressed";
	_mt["default"] = "application/octet-stream";
	return _mt;
}

const std::string ft::HttpUtils::getHttpFileType(std::string file)
{
	size_t dotPos = file.find_last_of(".");
	if (dotPos == std::string::npos)
		return _mimeTypes.at("default");
	std::string ext = file.substr(dotPos + 1);
	if (_mimeTypes.find(ext) == _mimeTypes.end())
		return _mimeTypes.at("default");
	return _mimeTypes.at(ext);
}

const std::string ft::HttpUtils::getHttpReason(unsigned int statusCode)
{
	switch (statusCode)
	{
		//####### 1xx - Informational #######
		case 100: return "Continue";
		case 101: return "Switching Protocols";
		case 102: return "Processing";

		//####### 2xx - Successful #######
		case 200: return "OK";
		case 201: return "Created";
		case 202: return "Accepted";
		case 203: return "Non-Authoritative Information";
		case 204: return "No Content";
		case 205: return "Reset Content";
		case 206: return "Partial Content";
		case 207: return "Multi-Status";
		case 208: return "Already Reported";
		case 226: return "IM Used";

		//####### 3xx - Redirection #######
		case 300: return "Multiple Choices";
		case 301: return "Moved Permanently";
		case 302: return "Found";
		case 303: return "See Other";
		case 304: return "Not Modified";
		case 305: return "Use Proxy";
		case 307: return "Temporary Redirect";
		case 308: return "Permanent Redirect";

		//####### 4xx - Client Error #######
		case 400: return "Bad Request";
		case 401: return "Unauthorized";
		case 402: return "Payment Required";
		case 403: return "Forbidden";
		case 404: return "Not Found";
		case 405: return "Method Not Allowed";
		case 406: return "Not Acceptable";
		case 407: return "Proxy Authentication Required";
		case 408: return "Request Timeout";
		case 409: return "Conflict";
		case 410: return "Gone";
		case 411: return "Length Required";
		case 412: return "Precondition Failed";
		case 413: return "Content Too Large";
		case 414: return "URI Too Long";
		case 415: return "Unsupported Media Type";
		case 416: return "Range Not Satisfiable";
		case 417: return "Expectation Failed";
		case 418: return "I'm a teapot";
		case 421: return "Misdirected Request";
		case 422: return "Unprocessable Content";
		case 423: return "Locked";
		case 424: return "Failed Dependency";
		case 425: return "Too Early";
		case 426: return "Upgrade Required";
		case 428: return "Precondition Required";
		case 429: return "Too Many Requests";
		case 431: return "Request Header Fields Too Large";
		case 451: return "Unavailable For Legal Reasons";

		//####### 5xx - Server Error #######
		case 500: return "Internal Server Error";
		case 501: return "Not Implemented";
		case 502: return "Bad Gateway";
		case 503: return "Service Unavailable";
		case 504: return "Gateway Timeout";
		case 505: return "HTTP Version Not Supported";
		case 506: return "Variant Also Negotiates";
		case 507: return "Insufficient Storage";
		case 508: return "Loop Detected";
		case 510: return "Not Extended";
		case 511: return "Network Authentication Required";

		default: return "";
	}

}

size_t ft::HttpUtils::checkHttpRequest(DataFd & data)
{
	HttpRequest &req = *data.httpRequest;
	std::string url = req.getUrl();
	std::string method = req.getMethod();
	const ALocation *location = data.configServer->getLocation(url);

	if (req.getHttpVersion() != "HTTP/1.1" && req.getHttpVersion() != "HTTP/1.0")
		return HTTP_VERSION_NOT_SUPPORTED;
	if (method != "GET" && method != "POST" && method != "DELETE" && method != "HEAD")
		return HTTP_BAD_REQUEST;
	// if (!location)
	// 		return HTTP_NOT_FOUND;
	// if (!(location->methodIsAllowed(method)))
	// 	return HTTP_METHOD_NOT_ALLOWED;
	data.loc = location;
	return HTTP_OK;
}

bool ft::HttpUtils::isSuccessful(size_t code)
{
	return (code >= HTTP_OK && code <= HTTP_IM_USED);
}

