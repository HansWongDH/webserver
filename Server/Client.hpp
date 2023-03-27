#include <iostream>

namespace ft{
	class Client {
		public:
			Client(){
				request = new std::string;
				respond = new std::string;
			};

			~Client() {
				// delete request;
				// delete respond;
			};

			Client(int server_fd) : server_fd(server_fd) {
				request = new std::string;
				respond = new std::string;
			}

			std::string	getRequest(){
				return *(this->request);
			}
			std::string	getRespond(){
				return *(this->respond);
			}
			bool	requestEmpty()
			{
				return request->empty();
			}

			bool	respondEmpty()
			{
				return respond->empty();
			}

			void	insertRequest(char *buf)
			{
				this->request->assign(buf, buf + strlen(buf));
				std::cout << "insertRequest: " << *this->request << std::endl;
			}

			void	insertRespond(char *buf)
			{
				// this->respond->assign(buf, buf + strlen(buf));
				std::string temp(buf);
				this->respond->insert(0, temp);
			}
			int	server_fd;
			std::string	*request;
			std::string *respond;
	};
}