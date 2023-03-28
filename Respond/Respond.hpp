#ifndef RESPOND_HPP
#define RESPOND_HPP
#include <iostream>
#include <sstream>
#include <fstream>
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
			const char	*_return;
		public:
			Respond(ft::ServerInfo info) : info(info), _respond(), size(0) {}
			~Respond(){}

			void	insertRespond(string infile)
			{
				this->_respond = infile;
				this->size += infile.size();
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

			void	methodGet(ft::Request *request)
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
				insertRespond(file_content);
			}

			void	methodPost(ft::Request *request)
			{
				
			}

			void	methodDelete(ft::Request *request)
			{
				
			}
			char* returnRespond(void)
			{
				char *ret;
				if (this->size > BUFFER_SIZE)
				{
					string tmp = _respond.substr(0, BUFFER_SIZE);
					size -= BUFFER_SIZE;
					// std::cout << "tmp here" << tmp << "size === " << size << std::endl;
					_respond.erase(0, BUFFER_SIZE);
					ret = const_cast<char *>(tmp.c_str());
				}
				else
				{
					size = 0;
					ret = const_cast<char *>(_respond.c_str());
					_respond.clear();
				}
				// std::cout << "==========="<< std::endl;
				return ret;
			}

			bool	empty()
			{
				if (size == 0)
					return true;
				return false;
			}


	};
} // namespace ft

#endif