#include "Response.hpp"

ft::Response::Response(ft::ServerInfo *info) : info(info), _response(), size(0), root("root"), auto_index(false)
{
}
ft::Response::~Response()
{

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

string ft::Response::responseHeader(int status_code)
{
	string status;
	
	if (status_code == OK)
		status = " 200 Ok";
	else if (status_code == NOT_FOUND)
		status = " 404 Not Found";
	else if (status_code == METHOD_NOT_ALLOWED)
		status = " 405 Method Not Allowed";
	else if (status_code == PAYLOAD_TOO_LARGE)
		status = " 413 Payload Too Large";
	else if (status_code == MOVE_PERMANENTLY)
		status = " 301 Move Permanently";
	else if (status_code == TEMPORARY_REDIRECT)
		status = " 307 Temporary Redirect";
	else if (status_code == PERMANENTLY_REDIRECT)
		status = " 308 Permanent Redirect";

	return ("HTTP/1.1" + status + "\r\nContent-Type: */*\r\n\r\n");
}

vector<string> splitUrl(string url)
{
	vector<string> ret;
	string token;
	stringstream ss(url);
	while (getline(ss, token, '/'))
		ret.push_back(token);
	return ret;
}

/**
 * @brief open direct url first
 * Exact match: Nginx first looks for an exact match between the request URL and a configured location block. If it finds a match, it sends the request to the corresponding backend server.

Preferential Prefix match: If no exact match is found, Nginx looks for a location block whose prefix matches the beginning of the request URL. If multiple location blocks have a matching prefix, Nginx chooses the one with the longest prefix.

Regular expression match: If no prefix match is found, Nginx looks for a location block that matches the request URL using a regular expression.

Default match: If no match is found, Nginx sends the request to the default server or displays a 404 error.	
 * 
 * @param request 
 */

void	ft::Response::PrefererentialPrefixMatch(ft::Request *request)
{
		try
		{
			info->getLocationKey(request->getPrefix());
			try
			{
				root = *info->getLocationInfo(request->getPrefix(), "root").begin();
			}
			catch(const std::exception& e)
			{
				std::cerr << e.what() << "root not found within" << request->getPrefix() << '\n';
			}

			try
			{
				if (!(*info->getLocationInfo(request->getPrefix(), "autoindex").begin()).compare("on"))
					auto_index = true;
			}
			catch(const std::exception& e)
			{
			}
			
			try
			{
				index = info->getLocationInfo(request->getPrefix(), "index");
			}
			catch(const std::exception& e)
			{
				try
				{
					index = info->getConfigInfo("index");
				}
				catch(const std::exception& e)
				{
					index.push_back("index.html");
				}
			}
		}
		catch(const std::exception& e)
		{
			this->status_code = NOT_FOUND;
		}
		
}

vector<string>	ft::Response::errorPage(void)
{
	vector<string> page;
	stringstream ss;
	ss << this->status_code;

	string status = ss.str();
	std::cout<< status << std::endl;
	try
	{
		page = info->getConfigInfo(status);
	}
	catch(const std::exception& e)
	{
		// std::cerr << "WHY KEY NOT FOUND?" << '\n';
	}
	return page;
	
}

string	autoIndexGenerator(ft::Request *request)
{
	string path = "root" + request->getPrefix();
	string response;
   	vector<std::pair<string, int> > dir_contents;
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(path.c_str())) != NULL) 
	{
        while ((ent = readdir(dir)) != NULL) 
		{    
                string file_path = path + ent->d_name;
                struct stat sb;
                stat(file_path.c_str(), &sb);
                dir_contents.push_back(std::make_pair(ent->d_name, sb.st_size));
        }
        closedir(dir);
	}

    // Sort the directory contents alphabetically by name
    // sort(dir_contents.begin(), dir_contents.end());

    // Write the HTTP response

    response = "<!DOCTYPE html>\n";
    response += "<html>\n";
    response += "<head>\n";
    response += "<title>Index of " + path + "</title>\n";
    response += "</head>\n";
    response += "<body>\n";
    response += "<h1>Index of " + path + "</h1>\n";
    response += "<hr>\n";
    response += "<table>\n";
    response += "<thead>\n";
    response += "<tr>\n";
    response += "<th>Name</th>\n";
    response += "<th>Size</th>\n";
    response += "</tr>\n";
    response += "</thead>\n";
    response += "<tbody>\n";

    for (vector<std::pair<string, int> >::const_iterator it = dir_contents.begin(); it != dir_contents.end(); ++it) {
        response += "<tr>\n";
        response += "<td><a href=\"" + request->getPrefix() + "/" +  it->first + "\">" +  it->first  + "</a></td>\n";
        response += "<td>" + std::to_string(it->second) + "</td>\n";
        response += "</tr>\n";
    }

    response += "</tbody>\n";
    response += "</table>\n";
    response += "<hr>\n";
    response += "</body>\n";
    response += "</html>\n";
    return response;
}

string	defaultErrorPage(void)
{
	string response;
	response = "<!DOCTYPE html>\n";
    response += "<html>\n";
    response += "<head>\n";
	response += "<title>404 not found</title>";
	response += "</head>\n";
	response += "<body>\n";
	response += "<h1> This is default error page </h1>\n";
	response += "</body>\n";
   	response += "</html>\n";

	return response;
}

void ft::Response::methodGet(ft::Request *request)
{
	std::cout << "Target: " <<  request->getTarget() << std::endl;
	try
	{
		root = info->getConfigInfo("root").front();
	}
	catch(const std::exception& e)
	{
	}
	std::fstream file;
	/*exact match*/
	file.open(root + request->getTarget());
	this->status_code = OK;
	/*if failed, do prefix matching*/
	if (!file.is_open())
	{
		
		PrefererentialPrefixMatch(request);
		for (ft::ServerInfo::iterator it = index.begin(); it != index.end(); it++)
		{
			file.open(root + request->getPrefix() + "/" + *it);
			if (file.is_open())
			{
				std::cout << " hi i am here" << std::endl;
				this->status_code = OK;
				break;
			}
		}
	}

	if (!file.is_open() || this->status_code == NOT_FOUND)
	{
		if (this->auto_index == true)
		{
			insertResponse(responseHeader(this->status_code).append(autoIndexGenerator(request)));
			return;
		}
		this->status_code = 404;
		if (!errorPage().empty())
			file.open(root + "/" + errorPage().back());
		if (!file.is_open() || errorPage().empty())
		{
			insertResponse(responseHeader(404).append(defaultErrorPage()));
			return ;
		}
	}
	string file_content((std::istreambuf_iterator<char>(file)),
						std::istreambuf_iterator<char>());
	insertResponse(responseHeader(this->status_code).append(file_content));
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
