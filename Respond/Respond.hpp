#ifndef RESPOND_HPP
#define RESPOND_HPP
#include <iostream>
#include <sstream>
#include <fstream>
#include <string.h> 
#include "../Request/Request.hpp"
#include "../Parser/Serverinfo.hpp"
#include "../includes/colour.h"
using std::string;

namespace ft
{
	class Respond{
		private:
			ft::ServerInfo	info;
			string _respond;
			int	size;
			char	_return[BUFFER_SIZE];
		public:
			Respond(ft::ServerInfo info) : info(info), _respond(), size(0) {}
			~Respond(){

			}

			void	insertRespond(string infile)
			{
				this->_respond = infile;
				this->size = infile.size();
			}
			
			void	parseRespond(ft::Request *request)
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

			string	headerGenerator(ft::Request *request)
			{
				string tmp = request->getVersion();
				tmp.append(" 200 OK \nContent-Type: */*\r\n");
				return tmp;
			}
			void	methodGet(ft::Request *request)
			{
				ft::ServerInfo::value_type vec = info.getLocationInfo(request->getTarget(), "index");
				std::fstream file;
				string root = ROOT;
				for (ft::ServerInfo::iterator it = vec.begin(); it != vec.end(); it++)
				{
					if (request->getTarget().compare("/"))
						file.open(root + request->getTarget() + "/" + *it);
					else
						file.open(root + request->getTarget() + *it);
					if (file.is_open())
						break ;
	
				}
				if (!file.is_open())
					file.open(root + "/404.html");
				string file_content((std::istreambuf_iterator<char>(file)),
                          std::istreambuf_iterator<char>());
				string tmp = headerGenerator(request);
				tmp.append(file_content);
				insertRespond(tmp);
			}

			void	methodPost(ft::Request *request)
			{
				
			}

			void	methodDelete(ft::Request *request)
			{
				
			}
			char* returnRespond(void)
			{
				if (this->size > BUFFER_SIZE)
				{
					string tmp = _respond.substr(0, BUFFER_SIZE);
					size -= BUFFER_SIZE;
				
					_respond.erase(0, BUFFER_SIZE);
					strlcpy(_return, tmp.c_str(), tmp.size());
				}
				else
				{
					strlcpy(_return, _respond.c_str(), _respond.size());
					std::cout << "what is the size? ====" << _respond.size() <<  " === " << strlen(_return) << std::endl;
					
					size = 0;
					_respond.clear();
				}
				return this->_return;
			}

			bool	empty()
			{
				if (size == 0)
					return true;
				return false;
			}

			int	getSize()
			{
				return this->size;
			}


	};
} // namespace ft

#endif