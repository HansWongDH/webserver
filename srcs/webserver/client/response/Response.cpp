#include "Response.hpp"

ft::Response::Response(ft::ServerInfo *info) : info(info), _response(), size(0)
{
	_return = (char *)malloc(sizeof(char) * BUFFER_SIZE);
}
ft::Response::~Response()
{
	if (_return)
		free(_return);
}

void ft::Response::insertResponse(string infile)
{
	this->_response = infile;
	this->size = infile.size();
}

void ft::Response::parseResponse(ft::Request *request)
{
	if (request->getMethod() == GET)
		methodGet(request);
	else if (request->getMethod() == POST)
		methodPost(request);
	else if (request->getMethod() == DELETE)
		methodDelete(request);
	else
		return;
}

string ft::Response::headerGenerator(ft::Request *request)
{
	(void)request;

	return ("HTTP/1.1 200 OK \r\nContent-Type: */*\r\n\r\n");
}

// string ft::Response::fileOpen(ft::Request *request)
// {
// 	std::fstream file;
// 	file.open(request->getTarget());
// 	if (!file.is_open())

// }

vector<string>	splitUrl(string url)
{
	vector<string> ret;
	string token;
	stringstream ss(url);
	while (getline(ss,token, '/'))
		if (!token.empty())
			ret.push_back(token);
	return ret;
}
void ft::Response::methodGet(ft::Request *request)
{
	std::cout << request->getTarget() << std::endl;
	string root = info->getConfigInfo("root").front();
	std::fstream file;
	file.open(root + request->getTarget());
	if (!file.is_open())
	{
		std::cout << "I enter here with" << request->getTarget() << std::endl;
		vector<string> content;
		vector<string> url = splitUrl(request->getTarget());
		
		for (vector<string>::iterator it = url.begin(); it != url.end(); it++)
			std::cout << *it << std::endl;
 		if (url.size() == 1)
			url.push_back("root");
		content = info->getLocationInfo("/" + url[0], url[1]);

		for (ft::ServerInfo::iterator it = content.begin(); it != content.end(); it++)
		{
			std::cout << root + "/" + url[0] + "/" + *it << std::endl;
			// std::cout << "HEREREREREREREE ============ " << root + request->getTarget() + *it << std::endl;
			file.open(root + "/" + url[0] + "/" + *it);
			// if (request->getTarget().back() == '/')
			// 	file.open(root + request->getTarget() + *it);
			// else if (request->getTarget().front() == '/' && request->getTarget().front() != request->getTarget().back())
			// 	file.open(root + request->getTarget() + "/" + *it);
			// else 
				// file.open(root + "/" + request->getTarget() + "/" + *it);
			if (file.is_open())
				break;
		}
	}
		if (!file.is_open())
			file.open(root + "/404.html");
		string file_content((std::istreambuf_iterator<char>(file)),
							std::istreambuf_iterator<char>());
		insertResponse(file_content);
		string tmp = headerGenerator(request);
		tmp.append(file_content);
		insertResponse(tmp);
}
void ft::Response::methodPost(ft::Request *request)
{
	(void)request;
}

void ft::Response::methodDelete(ft::Request *request)
{
	(void)request;
}

void ft::Response::returnResponse(int fd)
{
	if (this->size > BUFFER_SIZE)
	{
		string tmp = _response.substr(0, BUFFER_SIZE);
		size -= BUFFER_SIZE;
		// std::cout << "tmp here" << tmp << "size === " << size << std::endl;
		_response.erase(0, BUFFER_SIZE);
		write(fd, tmp.c_str(), BUFFER_SIZE);
		std::cout << "CURRENT SIZE === " << this->size << std::endl;
	}
	else
	{
		size = 0;
		write(fd, _response.c_str(), strlen(_response.c_str()) + 1);
		// std::cout << _response << std::endl;
		std::cout << "CURRENT SIZE === " << this->size << std::endl;
		_response.clear();
	}
}

bool ft::Response::empty()
{
	if (size == 0)
		return true;
	return false;
}
