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
	// if(this->request)
	// 	delete request;
	// if (this->response)
	// 	delete response;
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
	string tmp(buf);
	std::cout << "Strlen of buf :" << strlen(buf) << " | size of string after conversion :" << tmp.size() << std::endl;
	request->insertHeader(tmp, size);
}

void	ft::Client::parseRespond()
{
	response->parseResponse(this->request);
}
void	ft::Client::insertBody(char *buf, int size)
{

	string tmp(buf);
	// std::cout << "HERERERERERE" << strlen(buf) << "|" << tmp.size() << std::endl;
	request->insertBody(tmp, size);
}

void	ft::Client::parseHeader(int size)
{
	(void)size;
	request->parseHeader();
}
void	ft::Client::parseBody()
{
	request->parseBody();
	response->parseResponse(this->request);
}
