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
	ss >> url;
	requestPrefix();
	ss >> HTTPVersion;

}
string	ft::Request::getPrefix(void) const
{
	return this->prefix;
}
void	ft::Request::requestPrefix(void)
{
	string::iterator it = url.begin();
	while (it != url.end())
	{
		// std::cout << *it << std::endl;
		if (*it == '/' && it != url.begin())
			break;
		it++;
	}
	this->prefix = url.substr(0, it - url.begin());
}
string	ft::Request::getTarget(void) const
{
	return this->url;
}

int		ft::Request::getMethod(void)
{
	return this->method;
}
