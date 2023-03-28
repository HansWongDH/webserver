#ifndef RESPOND_HPP
#define RESPOND_HPP
#include <iostream>
#include "../Server/HTTPServer.hpp"

using std::string;
namespace ft
{
	class Respond{
		public:
			Respond() : _respond(), size(0) {}
			~Respond(){}

			void	insertRespond(string infile)
			{
				this->_respond = infile;
				this->size = infile.size();
			}
			
			const char* getRespond(void)
			{
				const char *ret;
				if (this->size > BUFFER_SIZE)
				{
					string tmp = _respond.substr(0, BUFFER_SIZE);
					size -= BUFFER_SIZE;
					_respond.erase(tmp.begin(), tmp.end());
					ret = tmp.c_str();
				}
				else
				{
					size = 0;
					ret = _respond.c_str();
					_respond.clear();
				}
				return ret;
			}

		private:
			string _respond;
			int	size;
			const char	*_return;
	};
} // namespace ft

#endif