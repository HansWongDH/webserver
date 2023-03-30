#include "Response.hpp"

ft::Response::Response(ft::ServerInfo* info) : info(info), _response(), size(0) {
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

string	ft::Response::headerGenerator(ft::Request *request)
			{
				(void)request;
		
				return ("HTTP/1.1 200 OK \r\nContent-Type: */*\r\n");
			}
void	ft::Response::methodGet(ft::Request *request)
			{
				ft::ServerInfo::value_type vec = info->getLocationInfo(request->getTarget(), "index");
				std::fstream file;
				string root = ROOT;
				for (ft::ServerInfo::iterator it = vec.begin(); it != vec.end(); it++)
				{
									// std::cout << "HEREREREREREREE ============ " << root + request->getTarget() + *it << std::endl;
					file.open(root + request->getTarget() + *it);
					if (request->getTarget().compare("/"))
						file.open(root + request->getTarget() + "/" + *it);
					else
						file.open(root + request->getTarget() + *it);
					if (file.is_open())
						break ;

				}
				if (!file.is_open())
					throw std::invalid_argument("Unable to find file");
					file.open(root + "/404.html");
				string file_content((std::istreambuf_iterator<char>(file)),
                          std::istreambuf_iterator<char>());
				insertResponse(file_content);
				string tmp = headerGenerator(request);
				tmp.append(file_content);
				insertResponse(tmp);
			}
void	ft::Response::methodPost(ft::Request *request)
{
	(void) request;
}

void	ft::Response::methodDelete(ft::Request *request)
{
	(void) request;
}

void	ft::Response::returnResponse(int fd)
{
	if (this->size > BUFFER_SIZE)
	{
		string tmp = _response.substr(0, BUFFER_SIZE);
		size -= BUFFER_SIZE;
		// std::cout << "tmp here" << tmp << "size === " << size << std::endl;
		_response.erase(0, BUFFER_SIZE + 1);
		write(fd, tmp.c_str(), BUFFER_SIZE);
		std::cout << tmp << std::endl;
	}
	else
	{
		size = 0;
		write(fd, _response.c_str(), _response.size());
		_response.clear();
	}
}

bool	ft::Response::empty()
{
	if (size == 0)
		return true;
	return false;
}
