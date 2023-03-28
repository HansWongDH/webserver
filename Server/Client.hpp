#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#
namespace ft{
	class Client {
		public:
			Client(){
				this->request = new std::string;
				this->respond = new std::string;
			}
			~Client() {
				// if(this->request)
				// 	delete request;
				// if (this->respond)
				// 	delete respond;
			};
			Client(int server_fd) : server_fd(server_fd) {
				this->request = new std::string;
				this->respond = new std::string;
			}
			
			int	Parse_request(void)
			{
				string temp;
				std::stringstream ss(getRequest());
				ss >> temp;
				std::cout << temp << std::endl;
				if (!temp.compare("GET"))
				{
								std::cout << temp << std::endl;
					return (1);
				}
				return (0);
			}
			std::string	getRequest() const {
				return *this->request;
			}
			std::string	getRespond() const {
				return *this->respond;
			}
			bool	requestEmpty()
			{
				return request->empty();
			}

			bool	respondEmpty()
			{
				return respond->empty();
			}

			// void	insertRequest(char *buf, size_t size)
			// {
			// 	this->request->clear();
			// 	this->request->append(buf, size);
			// 	std::cout << this->request << std::endl;
			// }
			

			// void	insertRespond(string buf)
			// {
			// 	this->respond->assign(buf.begin(), buf.end());
			// }
			int	server_fd;
			std::string	*request;
			std::string *respond;
	};
}

#endif