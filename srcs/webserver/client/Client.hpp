#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include "./request/Request.hpp"
#include "./response/Response.hpp"
#include "../server/ServerInfo.hpp"

namespace ft{
	class Client {
		public:
			// Client(ft::ServerInfo* info);
			Client(int server_fd, ft::ServerInfo info, const map<string,string>& env);
			~Client();
			
			ft::Request			*getRequest() const;
			ft::Response		*getResponse() const;
			bool				responseEmpty();
			void				insertRequest(char *buf);
			
			int					server_fd;
			ft::Request			*request;
			ft::Response		*response;
	};
}

#endif
