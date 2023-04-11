#include "Request.hpp"

ft::Request::Request() {};
ft::Request::~Request() {};

ft::Request::Request(string	header) : query_string(), body_string() {
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

string	ft::Request::getBody(void) const
{
	return this->body_string;
}

std::pair<const string, string> ft::Request::getCookie(void) const
{
	std::pair<const string, string> ret;
	if (headers.find("Cookie") == headers.end())
		return	std::make_pair("", "");
	return *headers.find("Cookie");
}
/**
 * @brief Decode URL
 * 
 * @param url 
 * @return string 
 */
string	urlDecode(string url)
{
	while (url.find("%20") != string::npos)
		url.replace(url.find("%20"), 3, " ");
	while (url.find("%24") != string::npos)
		url.replace(url.find("%24"), 3, "$");
	while (url.find("%26") != string::npos)
		url.replace(url.find("%26"), 3, "&");
	while (url.find("%0A") != string::npos)
		url.replace(url.find("%0A"), 3, "\n");
	while (url.find("%2B") != string::npos)
		url.replace(url.find("%2B"), 3, "+");
	while (url.find("%2C") != string::npos)
		url.replace(url.find("%2C"), 3, ",");
	while (url.find("%3A") != string::npos)
		url.replace(url.find("%3A"), 3, ":");
	while (url.find("%3B") != string::npos)
		url.replace(url.find("%3B"), 3, ";");
	while (url.find("%3D") != string::npos)
		url.replace(url.find("%3D"), 3, "=");
	while (url.find("%3F") != string::npos)
		url.replace(url.find("%3F"), 3, "?");
	while (url.find("%40") != string::npos)
		url.replace(url.find("%40"), 3, "@");
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
                params[key] = urlDecode(value);
            }
        }
		query_string = urlDecode(query);
    }
	url = urlDecode(url);

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
        // throw std::runtime_error("Invalid header end");
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

	std::string raw_body;
	if (headers.find("Transfer-Encoding") != headers.end() &&
		headers["Transfer-Encoding"] == "chunked") {
		// Read and reassemble chunks
		size_t chunk_start = raw_request.find("\r\n\r\n", headers_end) + 4;
		size_t chunk_end = 0;
		size_t chunk_size = 0;

		while (true) {
			chunk_end = raw_request.find("\r\n", chunk_start);
			std::istringstream iss(raw_request.substr(chunk_start, chunk_end - chunk_start));
			iss >> std::hex >> chunk_size;

			if (chunk_size == 0) {
				break;
			}

			chunk_start = chunk_end + 2;
			raw_body += urlDecode(raw_request.substr(chunk_start, chunk_size));
			chunk_start += chunk_size + 2;
		}
	} else {
		// Find start of body
		size_t body_start = headers_end + 4;

		// Parse body, if any
		if (raw_request.size() > body_start) {
			raw_body = urlDecode(raw_request.substr(body_start));
		}
	}

	if (headers.find("Content-Type") != headers.end() &&
		headers["Content-Type"] == "application/x-www-form-urlencoded") {
		std::string body_str = raw_body.substr(0);
		std::istringstream body_iss(body_str);
		std::string key_value_pair;
		while (std::getline(body_iss, key_value_pair, '&')) {
			size_t equals_sign = key_value_pair.find('=');
			if (equals_sign != std::string::npos) {
				std::string key = key_value_pair.substr(0, equals_sign);
				std::string value = key_value_pair.substr(equals_sign + 1);
				body[key] = urlDecode(value);
			}
		}
		body_string = urlDecode(body_str);
	} else if (headers.find("Content-Type") != headers.end() && headers["Content-Type"] == "application/json") {
		// Parse JSON data in body
		try {
			std::string body_str = raw_body.substr(0);
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
	else if (headers["Content-Type"].find("multipart/form-data") != std::string::npos) {
        // Find the boundary string
        std::string boundary_key = "boundary=";
        size_t boundary_pos = headers["Content-Type"].find(boundary_key);
        if (boundary_pos == std::string::npos) {
            throw std::runtime_error("No boundary specified in multipart/form-data request");
        }

        std::string boundary = headers["Content-Type"].substr(boundary_pos + boundary_key.length());
        boundary = "\r\n--" + boundary;

        size_t boundary_position = 0;
        size_t next_boundary_position = 0;

        while ((boundary_position = raw_body.find(boundary, next_boundary_position)) != std::string::npos) {
            next_boundary_position = raw_body.find(boundary, boundary_position + boundary.length());
            if (next_boundary_position == std::string::npos) {
                break;
            }
            
            std::string part = raw_body.substr(boundary_position + boundary.length(), next_boundary_position - boundary_position - boundary.length());

            // Find header_end
            size_t header_end = part.find("\r\n\r\n");
            if (header_end == std::string::npos) {
                // Invalid part
                continue;
            }

            // Parse headers
            std::istringstream headers_iss(part.substr(0, header_end));
            std::string header_line;
            std::string content_disposition;
            std::string name;
            std::string filename;

            while (std::getline(headers_iss, header_line, '\r')) {
                headers_iss.ignore(1); // Ignore trailing '\n' character
                size_t pos = header_line.find(':');
                if (pos == std::string::npos) {
                    // Invalid header line
                    continue;
                }
                std::string header_name = header_line.substr(0, pos);
                std::string header_value = header_line.substr(pos + 1);

                // Ignore leading whitespace in header value
                if (header_value.size() > 0 && header_value[0] == ' ') {
                    header_value = header_value.substr(1);
                }

                if (header_name == "Content-Disposition") {
                    content_disposition = header_value;
                    size_t name_pos = header_value.find("name=\"");
                    if (name_pos != std::string::npos) {
                        size_t name_end = header_value.find('\"', name_pos + 6);
                        if (name_end != std::string::npos) {
                            name = header_value.substr(name_pos + 6, name_end - name_pos - 6);
                        }
                    }

                    size_t filename_pos = header_value.find("filename=\"");
					                    if (filename_pos != std::string::npos) {
                        size_t filename_end = header_value.find('\"', filename_pos + 10);
                        if (filename_end != std::string::npos) {
                            filename = header_value.substr(filename_pos + 10, filename_end - filename_pos - 10);
                        }
                    }
                }
            }

			std::cout << "FILENAME: " << filename << std::endl;

            if (!filename.empty()) {
                // This part contains a file
                // Extract the binary file content
                std::string file_content = part.substr(header_end + 4);
				std::cout << "YAS: " << filename << std::endl;
                // Save the binary file content to a file
                std::ofstream file_output(filename, std::ios::binary);
                file_output.write(file_content.c_str(), file_content.size());
                file_output.close();
            } else if (!name.empty()) {
                // This part contains a form field
                std::string value = part.substr(header_end + 4);
                value = value.substr(0, value.find("\r\n")); // Remove trailing \r\n

                // Insert key-value pair into body map
                body[name] = value;

                if (body_string.length() > 0) {
                    body_string += "&";
                }
                body_string += name + "=" + value;
            }
        }
    }


	std::ofstream out("tmp.txt");
    out << raw_body;
    out.close();
    std::cout << "FOCUS: " << body_string << std::endl;

	requestPrefix();
}

std::string ft::Request::getContentType (void) const
{
	return this->contentType;
}