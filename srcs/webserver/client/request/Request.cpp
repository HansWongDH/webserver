#include "Request.hpp"

ft::Request::Request() {};
ft::Request::~Request() {};

ft::Request::Request(string	header) : query_string() {
	parse_request(header);
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

string	spaceConversion(string url)
{
	std::cout << "size before replacement :" << url.length() << std::endl;
	while (url.find("%20") != string::npos)
		url.replace(url.find("%20"), 3, " ");
	std::cout << "after replacement" << url.c_str() << std::endl;
	std::cout << "size after replacement :" << url.length() << std::endl;
	return url;
}

void	ft::Request::parse_request(const std::string& raw_request) {
	// Find first space character to separate method and URI
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
                params[key] = value;
            }
        }
		query_string = spaceConversion(query);
    }
	url = spaceConversion(url);


	// size_t query_start = url.find('?');
	// if (query_start != std::string::npos) {
	// 	this->query_string = url.substr(query_start + 1);
	// 	 url = url.substr(0, query_start);
	// }

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

    // Find start of body
    size_t body_start = headers_end + 4;

    // Parse body, if any
	if (raw_request.size() > body_start) {
		if (headers.find("Content-Type") != headers.end() &&
			headers["Content-Type"] == "application/x-www-form-urlencoded") {
			std::string body_string = raw_request.substr(body_start);
			std::istringstream body_iss(body_string);
			std::string key_value_pair;
			while (std::getline(body_iss, key_value_pair, '&')) {
				size_t equals_sign = key_value_pair.find('=');
				if (equals_sign != std::string::npos) {
					std::string key = key_value_pair.substr(0, equals_sign);
					std::string value = key_value_pair.substr(equals_sign + 1);
					body[key] = value;
				}
			}
		} else if (headers["Content-Type"] == "application/json") {
			// Parse JSON data in body
			try {
				std::string body_string = raw_request.substr(body_start);
				std::istringstream iss(body_string);
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
				}
			} catch (...) {
				throw std::runtime_error("Failed to parse JSON data");
			}
		}
	}

	requestPrefix();
}
/**
 * @brief convert %20 to space
 * 
 * @param url 
 * @return string 
 */
