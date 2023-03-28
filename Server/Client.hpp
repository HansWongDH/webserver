#include <iostream>

namespace ft{
	struct Request {
		std::string method;
		std::string uri;
		std::string version;
		std::string contentType;
		int contentLength;
		std::map<std::string, std::string> headers;
		std::map<std::string, std::string> body;
		std::map<std::string, std::string> params;
	};

	struct Response {
		int statusCode;
		std::string message;
		std::map<std::string, std::string> headers;
		std::string content;
	};

	class Client {
		public:
			Client(){
				this->request = new std::string;
				this->respond = new std::string;
			}
			~Client() {
				// if(this->request)
				// 	delete request;
				// if (this->respond)
				// 	delete respond;
			};
			Client(int server_fd) : server_fd(server_fd) {
				this->request = new std::string;
				this->respond = new std::string;
			}
			
			int	Parse_request(void)
			{
				std::string temp;
				std::stringstream ss(getRequest());
				ss >> temp;
				std::cout << temp << std::endl;
				if (!temp.compare("GET"))
				{
								std::cout << temp << std::endl;
					return (1);
				}
				return (0);
			}
			std::string	getRequest() const {
				return *this->request;
			}
			std::string	getRespond() const {
				return *this->respond;
			}
			bool	requestEmpty()
			{
				return request->empty();
			}

			bool	respondEmpty()
			{
				return respond->empty();
			}

			void	insertRequest(char *buf, size_t size)
			{
				this->request->clear();
				this->request->append(buf, size);
				std::cout << this->request << std::endl;
			}
			
			void	insertRespond(std::string buf)
			{
				this->respond->assign(buf.begin(), buf.end());
			}

			Request parse_request(const std::string& raw_request) {
				std::string request_str = raw_request;
				Request request;
				size_t pos = 0;
				
				// Parse request line
				pos = request_str.find(' ');
				if (pos == std::string::npos) {
					// Invalid request line
					throw std::runtime_error("Invalid request line");
				}
				request.method = request_str.substr(0, pos);
				request_str.erase(0, pos + 1);

				pos = request_str.find('?');
				if (pos != std::string::npos) {
					// Parse query string
					std::string query_str = request_str.substr(pos + 1);
					request.uri = request_str.substr(0, pos);
					request_str = query_str;
					
					while ((pos = request_str.find('&')) != std::string::npos) {
						std::string param_str = request_str.substr(0, pos);
						request_str.erase(0, pos + 1);
						
						pos = param_str.find('=');
						if (pos == std::string::npos) {
							// Invalid query parameter
							throw std::runtime_error("Invalid query parameter");
						}
						std::string param_name = param_str.substr(0, pos);
						std::string param_value = param_str.substr(pos + 1);
						request.params[param_name] = param_value;
					}
					
					pos = request_str.find('=');
					if (pos == std::string::npos) {
						// Invalid query parameter
						throw std::runtime_error("Invalid query parameter");
					}
					std::string param_name = request_str.substr(0, pos);
					std::string param_value = request_str.substr(pos + 1);
					request.params[param_name] = param_value;
				}

				pos = request_str.find(' ');
				if (pos == std::string::npos) {
					// Invalid request line
					throw std::runtime_error("Invalid request line");
				}
				request.version = request_str.substr(0, pos);
				request_str.erase(0, pos + 2);  // Skip CRLF

				// Parse headers
				while ((pos = request_str.find('\r')) != std::string::npos) {
					std::string header_line = request_str.substr(0, pos);
					request_str.erase(0, pos + 2);  // Skip CRLF
					
					if (header_line.empty()) {
						break;  // End of headers
					}
					
					pos = header_line.find(':');
					if (pos == std::string::npos) {
						// Invalid header line
						throw std::runtime_error("Invalid header line");
					}
					std::string header_name = header_line.substr(0, pos);
					std::string header_value = header_line.substr(pos + 2);
					request.headers[header_name] = header_value;
					
					if (header_name == "Content-Type") {
						request.contentType = header_value;
					} else if (header_name == "Content-Length") {
						request.contentLength = std::stoi(header_value);
					}
				}

				// Parse body
				if (!request.contentType.empty() && request.contentLength > 0) {
					std::string body_str = request_str.substr(0, request.contentLength);
					request_str.erase(0, request.contentLength);

					if (request.contentType == "application/x-www-form-urlencoded") {
						// Parse URL-encoded form data in body
						while ((pos = body_str.find('&')) != std::string::npos) {
							std::string param_str = body_str.substr(0, pos);
							body_str.erase(0, pos + 1);
							
							pos = param_str.find('=');
							if (pos == std::string::npos) {
								// Invalid form parameter
								throw std::runtime_error("Invalid form parameter");
							}
							std::string param_name = param_str.substr(0, pos);
							std::string param_value = param_str.substr(pos + 1);
							request.body[param_name] = param_value;
						}
						
						pos = body_str.find('=');
						if (pos == std::string::npos) {
							// Invalid form parameter
							throw std::runtime_error("Invalid form parameter");
						}
						std::string param_name = body_str.substr(0, pos);
						std::string param_value = body_str.substr(pos + 1);
						request.body[param_name] = param_value;
					} else if (request.contentType == "application/json") {
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
								size_t next_pos = json_str.find(',', pos + 1);
								if (next_pos == std::string::npos) {
									next_pos = json_str.size() - 1;
								}
								value = json_str.substr(pos + 1, next_pos - pos - 1);

								// Remove quotes around value
								if (value[0] == '"' && value[value.size() - 1] == '"') {
									value = value.substr(1, value.size() - 2);
								}

								// Insert key-value pair into body map
								request.body[key] = value;
								pos = next_pos + 1;
							}
						} catch (...) {
							throw std::runtime_error("Failed to parse JSON data");
						}
					}
				}
				return request;
			}

			int	server_fd;
			std::string	*request;
			std::string *respond;
	};
}