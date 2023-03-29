#ifndef RESPONSE_HPP
#define RESPONSE_HPP
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include "../Request/Request.hpp"
#include "../../server/ServerInfo.hpp"
#include "../../../../includes/colour.h"

using std::string;

namespace ft
{
	class Response{
		public:
			Response(ft::ServerInfo info);
			~Response();

			void	insertResponse(string infile);
			void	parseResponse(ft::Request *request);
			void	methodGet(ft::Request *request);
			void	methodPost(ft::Request *request);
			void	methodDelete(ft::Request *request);
			char*	returnResponse(void);
			bool	empty();

		private:
			ft::ServerInfo	info;
			string			_response;
			int				size;
			char			*_return;
	};
} // namespace ft

#endif