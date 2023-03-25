#include <iostream>

namespace ft{
	struct Client {
			Client(){};
			~Client() {};
			Client(int server_id) : server_id(server_id) {}

			int	getRequest(){}
			int	getRespond(){}
			int	server_id;
			std::string	request;
			std::string respond;
	};
}