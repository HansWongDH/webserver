#ifndef REQUEST_HPP
#define REQUEST_HPP
#include <iostream>
#include <sstream>
using std::string;

namespace ft{
	class Request{
		public:
			Request();
			~Request();
			Request(string	header);
			
			void	parse_header(string header);
			string	getTarget(void) const;
			string	getPrefix(void) const;
			string	getMethod(void);

		private:
			string	method;
			string	url;
			string	prefix;
			string	HTTPVersion;
			string	body;

			void	requestPrefix(void);
	};
}
#endif