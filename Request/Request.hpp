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

#define ROOT "root";

namespace ft{
	class Request{
		public:
			Request();
			~Request();
			Request(string	header){
				parse_header(header);
			}
			void	parse_header(string header)
			{
				std::stringstream ss(header);
				string key;

				ss >> key;
				if (!key.compare("GET"))
					method = GET;
				else if (!key.compare("POST"))
					method = POST;
				else if (!key.compare("DELETE"))
					method = DELETE;
				else
				{
					method = 0;
					throw std::invalid_argument("Invalid Method");
				}
				string root = ROOT;

				ss >> target;
				target = root + target;
			}

		private:
			int	method;
			string target;
			string body;

	}
}
#endif