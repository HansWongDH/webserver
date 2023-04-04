#include "Client.hpp"

// ft::Client::Client(ft::ServerInfo info){
// 	this->request = new ft::Request;
// 	this->response = new ft::Response(info);
// }

ft::Client::Client(int server_fd, ft::ServerInfo info, const map<string,string>& envs) : server_fd(server_fd) {
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

void	ft::Client::insertRequest(char *buf)
{
	string tmp = buf;
	request->parse_request(tmp);
	response->parseResponse(this->request);
}
