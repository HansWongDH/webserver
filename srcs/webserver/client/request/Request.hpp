#ifndef REQUEST_HPP
#define REQUEST_HPP
#include <iostream>
#include <sstream>
using std::string;

enum {
	GET = 1,
	POST = 2,
	DELETE = 3
};

namespace ft{
	class Request{
		public:
			Request();
			~Request();
			Request(string	header);
			
			void	parse_header(string header);
			string	getTarget(void) const;
			int		getMethod(void);

		private:
			int		method;
			string	target;
			string	body;
	};
}
#endif