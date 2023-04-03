#include "Response.hpp"
#include "helper.hpp"

ft::Response::Response(ft::ServerInfo info) : info(info), _response(), size(0), root("root"), auto_index(false), redirection(false)
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

/**
 * @brief fetch and initalize info such as "root, allowed_method, index" from locationblock
 * if failed, initalized with the server block config instead
 *
 * @param prefix
 * @param value
 * @return vector<string>
 */
vector<string> ft::Response::initalizeLocationConfig(string prefix, string value)
{
	vector<string> ret;

	try
	{
		ret = info.getLocationInfo(prefix, value);
	}
	catch (const std::exception &e)
	{
		std::cerr << value << " is not specified in " << prefix << " initalized with server config" << '\n';
		try
		{
			ret = info.getConfigInfo(value);
		}
		catch (const std::exception &e)
		{
			std::cerr << value << " is not specified in server, initalized with default config" << '\n';
		}
	}
	return ret;
}
int ft::Response::allowedMethod(ft::Request *request)
{
	vector<string> allowed_method;
	if (!prefererentialPrefixMatch(request->getTarget()).empty())
	{
		allowed_method = initalizeLocationConfig(request->getPrefix(), "allow_methods");
		for (vector<string>::iterator it = allowed_method.begin(); it != allowed_method.end(); it++)
			if (!request->getMethod().compare(*it))
				return OK;
	}
	else
	{
		return NOT_FOUND;
	}
	return METHOD_NOT_ALLOWED;
}
void ft::Response::parseResponse(ft::Request *request)
{
	this->status_code = allowedMethod(request);
	if (this->status_code != OK)
	{
		insertResponse(responseHeader(this->status_code).append(errorPage()));
		return;
	}
	else
	{
		if (request->getMethod() == "GET")
			methodGet(request);
		else if (request->getMethod() == "POST")
			methodPost(request);
		else if (request->getMethod() == "DELETE")
			methodDelete(request);
		else
			return;
	}
}

string getStatus(int status_code)
{
	switch (status_code)
	{
	case OK:
		return " 200 Ok";
	case NOT_FOUND:
		return " 404 Not Found";
	case METHOD_NOT_ALLOWED:
		return " 405 Method Not Allowed";
	case PAYLOAD_TOO_LARGE:
		return " 413 Payload Too Large";
	case MOVED_PERMANENTLY:
		return " 301 Moved Permanently";
	case TEMPORARY_REDIRECT:
		return " 307 Temporary Redirect";
	case PERMANENTLY_REDIRECT:
		return " 308 Permanent Redirect";
	default:
		return " 500 Internal Server Error";
	}
}

string ft::Response::responseHeader(int status_code)
{
	return ("HTTP/1.1" + getStatus(status_code) + "\r\nContent-Type: */*\r\n\r\n");
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

string ft::Response::prefererentialPrefixMatch(string url)
{
	if (info.getLocationCount(url))
		return url;
	else
	{
		// std::cout << " I enter here w/ url ==== " << url << std::endl;
		if (!url.find_last_of('/'))
			return ("");
		else
			return prefererentialPrefixMatch(url.substr(0, url.find_last_of('/')));
	}
}
string ft::Response::errorPage(void)
{
	vector<string> page;
	stringstream ss;
	ss << this->status_code;

	string status = ss.str();
	std::fstream file;
	std::cout << status << std::endl;
	if (info.getConfigCount(status))
	{
		page = info.getConfigInfo(status);
		for (vector<string>::iterator it = page.begin(); it != page.end(); it++)
		{
			std::cout << "I am appending here" << ft::pathAppend(root, *it) << std::endl;
			file.open(ft::pathAppend(root, *it));
			if (file.is_open())
				return (string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>()));
		}
	}
	return defaultErrorPage();
}

string	ft::Response::pageRedirection(string target)
{
	string prefix = prefererentialPrefixMatch(target);
	try
	{
		if (!info.getLocationInfo(prefix, "return").empty())
		{
			std::cout << BLUE "redirection prefix " << prefix << "redirection substr =" << target.substr(target.find(prefix) + prefix.length()) << RESET << std::endl;
			string redir = pathAppend(info.getLocationInfo(prefix, "return").front(), target.substr(target.find(prefix) + prefix.length()));
			redirection = true;
			std::cout << "Redirection exist, redirecting to " << redir << std::endl;
			return redir;
		}
	}
	catch(const std::exception& e)
	{	
	}
	return target;
}

string ft::Response::autoIndexGenerator(string prefix)
{
	string path = ft::pathAppend(root, prefix);
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

	for (vector<std::pair<string, int> >::const_iterator it = dir_contents.begin(); it != dir_contents.end(); ++it)
	{
		response += "<tr>\n";
		response += "<td><a href=\"" + prefix + "/" + it->first + "\">" + it->first + "</a></td>\n";
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

string ft::Response::defaultErrorPage(void)
{
	string response;
	response = "<!DOCTYPE html>\n";
	response += "<html>\n";
	response += "<head>\n";
	response += "<title>This is default error page of" + getStatus(this->status_code) + "</title>";
	response += "</head>\n";
	response += "<body>\n";
	response += "<h1>" + getStatus(this->status_code) + " </h1>\n";
	response += "</body>\n";
	response += "</html>\n";

	return response;
}

void ft::Response::methodGet(ft::Request *request)
{
	std::cout << "here" << this->status_code << std::endl;
	std::cout << "Target: " << request->getTarget() << std::endl;
	try
	{
		root = info.getConfigInfo("root").front();
	}
	catch (const std::exception &e)
	{
	}
	std::fstream file;
	/*exact match*/

	string target = pageRedirection(request->getTarget());
	file.open(ft::pathAppend(root, target));
	string prefix = prefererentialPrefixMatch(target);
	/*if failed, do prefix matching*/


	if (!file.is_open())
	{
		if (!prefix.empty())
		{
			if (!initalizeLocationConfig(prefix, "root").empty())
				this->root = initalizeLocationConfig(prefix, "root").front();
			if (!initalizeLocationConfig(prefix, "index").empty())
				this->index = initalizeLocationConfig(prefix, "index");
			if (!initalizeLocationConfig(prefix, "autoindex").empty())
			{
				if (!initalizeLocationConfig(prefix, "autoindex").front().compare("on"))
					this->auto_index = true;
			}
			for (ft::ServerInfo::iterator it = index.begin(); it != index.end(); it++)
			{
				file.open(ft::pathAppend(root, prefix, *it));
				if (file.is_open())
				{
					if (redirection == false)
						this->status_code = OK;
					else
						this->status_code = MOVED_PERMANENTLY;
					break;
				}
			}
		}
		else
			this->status_code = NOT_FOUND;
	}
	// std::cout << "current status code" << this->status_code << std::endl;
	if (!file.is_open())
		this->status_code = NOT_FOUND;
	if (this->status_code != OK)
	{
		if (this->auto_index == true)
			insertResponse(responseHeader(this->status_code).append(autoIndexGenerator(prefix)));
		else
			insertResponse(responseHeader(this->status_code).append(errorPage()));
	}
	else
		insertResponse(responseHeader(this->status_code).append(string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>())));
}
void ft::Response::methodPost(ft::Request *request)
{
	(void)request;
}

bool isSubdirectory(const std::string &request_path)
{
	const std::string &root = "root";
	char *root_dir = realpath(root.c_str(), NULL);

	// Get the absolute path of the request
	char *abs_path = realpath(request_path.c_str(), NULL);
	if (abs_path == NULL)
	{
		std::cerr << "Error resolving path " << request_path << std::endl;
		return false;
	}

	// Check if the absolute path starts with the root directory path and is not the root directory itself
	bool is_subdir = (strcmp(root_dir, abs_path) != 0) && (strncmp(root_dir, abs_path, strlen(root_dir)) == 0);
	free(abs_path);
	free(root_dir);
	return is_subdir;
}

// Recursively delete all files and subdirectories under a given directory
int delete_directory(const std::string &path)
{
	DIR *dir = opendir(path.c_str());
	if (dir == NULL)
	{
		// Failed to open directory
		return 1;
	}

	dirent *entry;
	while ((entry = readdir(dir)) != NULL)
	{
		// Ignore '.' and '..' directories
		if (std::string(entry->d_name) == "." || std::string(entry->d_name) == "..")
		{
			continue;
		}

		std::string full_path = path + "/" + std::string(entry->d_name);

		struct stat stat_buf;
		if (lstat(full_path.c_str(), &stat_buf) == -1)
		{
			// Failed to stat file or directory
			continue;
		}

		if (S_ISDIR(stat_buf.st_mode))
		{
			// Recursively delete subdirectory
			if (delete_directory(full_path) != 0)
			{
				closedir(dir);
				return 1;
			}
			if (rmdir(full_path.c_str()) != 0)
			{
				closedir(dir);
				return 1;
			}
		}
		else
		{
			// Delete file
			if (remove(full_path.c_str()) != 0)
			{
				closedir(dir);
				return 1;
			}
		}
	}

	closedir(dir);

	// Delete directory itself
	if (rmdir(path.c_str()) != 0)
	{
		return 1;
	}

	return 0;
}

void ft::Response::methodDelete(ft::Request *request)
{
	std::cout << "Target: " << request->getTarget() << std::endl;
	try
	{
		root = info.getConfigInfo("root").front();
	}
	catch (const std::exception &e)
	{
	}

	// Ensure we dont allow someone to delete the root directory or go above & also check if file exists
	std::string request_path = root + request->getTarget();
	if (!isSubdirectory(request_path.c_str()) || access(request_path.c_str(), F_OK))
	{
		this->status_code = NOT_FOUND;
		insertResponse(responseHeader(this->status_code).append("Not Found!"));
		return;
	}
	int result = remove(request_path.c_str());

	if (result == 0)
	{
		this->status_code = OK;
		insertResponse(responseHeader(this->status_code).append("Deleted!"));
	}
	else
	{
		std::cerr << "Unable to delete file" << std::endl;
		result = delete_directory(request_path);

		if (result == 0)
		{
			this->status_code = OK;
			insertResponse(responseHeader(this->status_code).append("Deleted!"));
		}
		else
		{
			std::cerr << "Unable to delete dir" << std::endl;
			this->status_code = INTERNAL_SERVER_ERROR;
			insertResponse(responseHeader(this->status_code).append("Internal server error"));
		}
	}
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
		write(fd, _response.c_str(), _response.size());
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
