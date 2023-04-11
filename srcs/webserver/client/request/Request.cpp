#include "Request.hpp"

ft::Request::Request() : content_length(-1), raw_bytes(0) {};
ft::Request::~Request() {};

// ft::Request::Request(string	header) : query_string(), body_string() {
// 	std::cout << "HERE" << std::endl;
// }

void	ft::Request::insertHeader(const string& raw_request, int size)
{
	this->raw_bytes += size;
	this->_header.append(raw_request);
}

#include  <string.h>

void	ft::Request::insertBody(const string& raw_request, int size)
{
	this->_body.append(raw_request);
	this->content_length -= size;
}


int	ft::Request::findCarriage(void) const
{
	if (this->_header.find("\r\n\r\n") != std::string::npos)
		return true;
	return false;
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

string		ft::Request::getMethod(void)
{
	return this->method;
}

std::map<string, string>	ft::Request::getParams(void)
{
	return this->params;
}

string	ft::Request::getQuery(void) const
{
	return this->query_string;
}

string	ft::Request::getBody(void) const
{
	return this->_body;
}
std::pair<const string, string> ft::Request::getCookie(void) const
{
	std::pair<const string, string> ret;
	if (headers.find("Cookie") == headers.end())
		return	std::make_pair("", "");
	return *headers.find("Cookie");
}

int		ft::Request::getcontentLength(void) const
{
	return (this->content_length);
}
/**
 * @brief convert %20 to space
 * 
 * @param url 
 * @return string 
 */
string	spaceConversion(string url)
{
	while (url.find("%20") != string::npos)
		url.replace(url.find("%20"), 3, " ");
	return url;
}

void	ft::Request::eraseBody(size_t pos, size_t size)
{
	this->_body.erase(pos, size);
}

void	ft::Request::parseHeader() {
	// Find first space character to separate method and URI
	string raw_request = this->_header;

	this->_body = raw_request.substr(raw_request.find("\r\n\r\n") + 4);
	// std::cout << "HERERERERE=== " << 	this->_header.size() << "herere" <<  size << "|||" <<this->_body.size() << std::endl;
    size_t method_end = raw_request.find(' ');
    if (method_end == std::string::npos) {
        // Invalid request
        throw std::runtime_error("Invalid request line");
    }


    // Parse method
    method = raw_request.substr(0, method_end);

    // Find second space character to separate URI and HTTP version
    size_t uri_end = raw_request.find(' ', method_end + 1);
    if (uri_end == std::string::npos) {
        // Invalid request
        throw std::runtime_error("Invalid request line");
    }

    // Parse URI
    url = raw_request.substr(method_end + 1, uri_end - method_end - 1);
	
    // Parse query string, if any
    size_t query_start = url.find('?');
    if (query_start != std::string::npos) {
        std::string query = url.substr(query_start + 1);
        url = url.substr(0, query_start);
        std::istringstream iss(query);
        std::string key_value_pair;
        while (std::getline(iss, key_value_pair, '&')) {
            size_t equals_sign = key_value_pair.find('=');
            if (equals_sign != std::string::npos) {
                std::string key = key_value_pair.substr(0, equals_sign);
                std::string value = key_value_pair.substr(equals_sign + 1);
                params[key] = spaceConversion(value);
            }
        }
		query_string = spaceConversion(query);
    }
	url = spaceConversion(url);

    // Parse HTTP version
    version = raw_request.substr(uri_end + 1, raw_request.find('\r', uri_end) - uri_end - 1);

    // Find start of headers
    size_t headers_start = raw_request.find("\r\n") + 2;
    if (headers_start == std::string::npos) {
        // Invalid request
        throw std::runtime_error("Invalid header start");
    }

    // Find end of headers
    size_t headers_end = raw_request.find("\r\n\r\n", headers_start);
    if (headers_end == std::string::npos) {
        // Invalid request
        throw std::runtime_error("Invalid header end");
    }

    // Parse headers
    std::istringstream headers_iss(raw_request.substr(headers_start, headers_end - headers_start));
    std::string header_line;
    while (std::getline(headers_iss, header_line, '\r')) {
        // Ignore trailing '\n' character
        headers_iss.ignore(1);

        size_t pos = header_line.find(':');
        if (pos == std::string::npos) {
            // Invalid header line
            throw std::runtime_error("Invalid request line");
        }
        std::string header_name = header_line.substr(0, pos);
        std::string header_value = header_line.substr(pos + 1);
        // Ignore leading whitespace in header value
        if (header_value.size() > 0 && header_value[0] == ' ') {
            header_value = header_value.substr(1);
        }
        headers[header_name] = header_value;
    }

	if (headers.find("Content-Length") != headers.end())
	{
		std::stringstream ss(headers.find("Content-Length")->second);
		ss >> this->content_length;
	}
	else
	 	this->content_length = 0;

	std::cout << "raw_bytes ===  " << raw_bytes << "content length === " << content_length << std::endl;
	this->content_length -= (raw_bytes - (raw_request.find("\r\n\r\n") + 4));
	
}

void	ft::Request::parseBody()
{
	// Find start of body

    // Parse body, if any
	if (!this->_body.empty()) {
		body_string = spaceConversion(this->_body);
		std::string body_str = this->_body;
		if (headers.find("Content-Type") != headers.end() &&
			headers["Content-Type"] == "application/x-www-form-urlencoded") {

			std::istringstream body_iss(body_str);
			std::string key_value_pair;
			while (std::getline(body_iss, key_value_pair, '&')) {
				size_t equals_sign = key_value_pair.find('=');
				if (equals_sign != std::string::npos) {
					std::string key = key_value_pair.substr(0, equals_sign);
					std::string value = key_value_pair.substr(equals_sign + 1);
					body[key] = spaceConversion(value);
				}
			}
			body_string = spaceConversion(body_str);
		} else if (headers["Content-Type"] == "application/json") {
			// Parse JSON data in body
			try {
				std::istringstream iss(body_str);
				std::stringstream ss;
				ss << iss.rdbuf();

				std::string json_str = ss.str();
				size_t pos = 0;
				std::string key, value;

				// Parse key-value pairs from JSON string
				while (pos < json_str.size()) {
					// Find next key
					pos = json_str.find('"', pos);
					if (pos == std::string::npos) {
						break;
					}
					size_t end_pos = json_str.find('"', pos + 1);
					if (end_pos == std::string::npos) {
						break;
					}
					key = json_str.substr(pos + 1, end_pos - pos - 1);

					// Find next value
					pos = json_str.find(':', end_pos);
					if (pos == std::string::npos) {
						break;
					}
					size_t next_pos = json_str.find_first_of(",\n", pos + 1);
					if (next_pos == std::string::npos) {
						next_pos = json_str.size() - 1;
					}
					value = json_str.substr(pos + 1, next_pos - pos - 1);

					// Remove leading space
					if (value[0] == ' ')
						value = value.substr(1);

					// Remove quotes around value if present
					if (value[0] == '"' && value[value.size() - 1] == '"') {
						value = value.substr(1, value.size() - 2);
					}

					// Insert key-value pair into body map
					body[key] = value;
					pos = next_pos + 1;
					if (body_string.length() > 0)
						body_string += "&";
					body_string += key + "=" + value;
				}
			} catch (...) {
				throw std::runtime_error("Failed to parse JSON data");
			}
		}
	}
}
std::string ft::Request::getContentType (void) const
{
	return this->contentType;
}