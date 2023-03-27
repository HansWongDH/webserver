#include <iostream>

namespace ft{
	struct Client {
			Client(){};
			~Client() {};
			Client(int server_fd) : server_fd(server_fd) {}

			std::string	getRequest(){
				return this->request;
			}
			std::string	getRespond(){
				return this->respond;
			}
			bool	requestEmpty()
			{
				return request.empty();
			}

			bool	respondEmpty()
			{
				return respond.empty();
			}

			void	insertRequest(char *buf)
			{
				this->request.assign(buf, buf + strlen(buf));
			}

			void	insertRespond(char *buf)
			{
				this->respond.assign(buf, buf + strlen(buf));
			}
			int	server_fd;
			std::string	request;
			std::string respond;
	};
}