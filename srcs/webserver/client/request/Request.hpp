#ifndef REQUEST_HPP
#define REQUEST_HPP
#include <iostream>
#include <sstream>
#include <map>
using std::string;

namespace ft{
	class Request{
		public:
			Request();
			~Request();
			Request(string	header);
			
			void	parse_request(const string& raw_request);
			string	getTarget(void) const;
			string	getPrefix(void) const;
			string	getMethod(void);

		private:
			string	method;
			string	url;
			string	prefix;
			string	HTTPVersion;
			string	version;
			string	contentType;
			std::map<string, string> headers;
			std::map<string, string> body;
			std::map<string, string> params;

			void	requestPrefix(void);
	};
}
#endif