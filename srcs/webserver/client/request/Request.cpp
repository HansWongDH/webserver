#include "Request.hpp"

ft::Request::Request() {};
ft::Request::~Request() {};
ft::Request::Request(string	header){
	parse_header(header);
}
void	ft::Request::parse_header(string header)
{
	std::stringstream ss(header);
	string key;

	ss >> key;
	if (!key.compare("GET"))
		method = GET;
	else if (!key.compare("POST"))
		method = POST;
	else if (!key.compare("DELETE"))
		method = DELETE;
	else
	{
		method = 0;
		throw std::invalid_argument("Invalid Method");
	}
	ss >> target;

}

string	ft::Request::getTarget(void) const
{
	return this->target;
}

int		ft::Request::getMethod(void)
{
	return this->method;
}
