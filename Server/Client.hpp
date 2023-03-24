#include <iostream>

namespace ft{
	class Client {
		public:
			Client(){};
			~Client() {};
			Client(int fd) : server_fd(fd) {}
		private:
			int	client_fd;
			int	server_fd;
			std::string	request;
			std::string respond;
	};
}