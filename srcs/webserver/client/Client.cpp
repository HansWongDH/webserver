#include "Client.hpp"

// ft::Client::Client(ft::ServerInfo info){
// 	this->request = new ft::Request;
// 	this->response = new ft::Response(info);
// }

ft::Client::Client(int server_fd, ft::ServerInfo* info, const map<string,string>& envs) : server_fd(server_fd) {
	this->request = new ft::Request;
	this->response = new ft::Response(info, envs);
}

ft::Client::~Client() {
	delete request;
	delete response;
};
			
ft::Request*	ft::Client::getRequest() const {
	return this->request;
}
ft::Response*	ft::Client::getResponse() const {
	return this->response;
}

bool	ft::Client::responseEmpty()
{
	return response->empty();
}
void	ft::Client::insertHeader(char *buf, int size)
{
	request->insertHeader(string(buf, size));
	if (request->findCarriage() == true)
	{
		std::cout << "header found" <<std::endl;
		request->parseHeader();
	}
}

void	ft::Client::parseRespond()
{
	response->parseResponse(this->request);
}
void	ft::Client::insertBody(char *buf, int size)
{
	request->insertBody(string(buf, size));
}

void	ft::Client::parseBody()
{
	request->parseBody();
	response->parseResponse(this->request);
}
