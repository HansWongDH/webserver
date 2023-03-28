#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include "../Request/Request.hpp"
#include "../Respond/Respond.hpp"
#include "../Parser/Serverinfo.hpp"
namespace ft{
	class Client {
		public:
			Client(ft::ServerInfo info){
				this->request = new ft::Request;
				this->respond = new ft::Respond(info);
			}
			~Client() {
				// delete this->request;
				// delete this->respond;
			}
			Client(int server_fd, ft::ServerInfo info) : server_fd(server_fd) {
				this->request = new ft::Request;
				this->respond = new ft::Respond(info);
			}
			
			ft::Request	*getRequest() const {
				return this->request;
			}
			ft::Respond	*getRespond() const {
				return this->respond;
			}

			// bool	requestEmpty()
			// {
			// 	return request->empty();
			// }

			bool	respondEmpty()
			{
				return respond->empty();
			}

			void	insertRequest(char *buf)
			{
				string tmp = buf;
				request->parse_header(tmp);
				respond->parseRespond(this->request);
								request->requestClear();
			
				// std::cout << this->request << std::endl;
			}
			

			// void	insertRespond(string buf)
			// {
			// 	this->respond->assign(buf.begin(), buf.end());
			// }
			int	server_fd;
			ft::Request		*request;
			ft::Respond		*respond;
	};
}

#endif