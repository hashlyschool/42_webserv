#include <string>
#include "./HttpRequest.hpp"

namespace ft
{
	class HttpResponse
	{
		/* invalid cases:
		if there is a whitespace between start line and next line 
		-> either reject the request or skip the following line

		if no host -> case invalid
		*/

	/* schema -> 1. check request for errors
				2. reprocess information from request to response
				(e.g. url to normalized url and so on)
				3. do everything according to method from request
				4. catch any errors from the previous step
				5. form a response http message*/

		private:
			unsigned int _responseCode;
			bool		_chunked;
			
		public:
			HttpResponse();
			HttpResponse &operator=(const HttpResponse &rhs);
			void generateResponse(HttpRequest request);
	};
}