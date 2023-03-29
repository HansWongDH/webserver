#include "Response.hpp"

ft::Response::Response(ft::ServerInfo info) : info(info), _response(), size(0) {
	_return = (char *)malloc(sizeof(char) * BUFFER_SIZE);
}
ft::Response::~Response(){
	if (_return)
		free(_return);
}

void	ft::Response::insertResponse(string infile)
{
	this->_response = infile;
	this->size += infile.size();
}

void	ft::Response::parseResponse(ft::Request *request)
{
	if (request->getMethod() == GET)
		methodGet(request);
	else if (request->getMethod() == POST)
		methodPost(request);
	else if (request->getMethod() == DELETE)
		methodDelete(request);
	else
		return ;
}

void	ft::Response::methodGet(ft::Request *request)
{
	ft::ServerInfo::value_type vec = info.getLocationInfo(request->getTarget(), "index");
	std::fstream file;
	string root = ROOT;
	for (ft::ServerInfo::iterator it = vec.begin(); it != vec.end(); it++)
	{
		file.open(root + request->getTarget() + *it);
		if (file.is_open())
			break ;
	}
	if (!file.is_open())
		throw std::invalid_argument("Unable to find file");
	string file_content((std::istreambuf_iterator<char>(file)),
				std::istreambuf_iterator<char>());
	insertResponse(file_content);
}

void	ft::Response::methodPost(ft::Request *request)
{
	
}

void	ft::Response::methodDelete(ft::Request *request)
{
	
}
char*	ft::Response::returnResponse(void)
{
	char *ret;
	if (this->size > BUFFER_SIZE)
	{
		string tmp = _response.substr(0, BUFFER_SIZE);
		size -= BUFFER_SIZE;
		// std::cout << "tmp here" << tmp << "size === " << size << std::endl;
		_response.erase(0, BUFFER_SIZE + 1);
		ret = const_cast<char *>(tmp.c_str());
	}
	else
	{
		size = 0;
		ret = const_cast<char *>(_response.c_str());
		_response.clear();
	}
	_return = strdup(ret);
	return _return;
}

bool	ft::Response::empty()
{
	if (size == 0)
		return true;
	return false;
}
