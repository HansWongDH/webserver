#include "./Server/Server.hpp"
#include "./Parser/Parser.hpp"
#include <iostream>
#include <vector>
#include <poll.h>

using std::cout;
using std::endl;
using std::vector;

int main(void)
{
	std::ifstream file;
	file.open("./config/example.conf");
	const char *temp[] = {"listen", "server_name", "root"};
	std::vector<std::string> test(temp, temp + 3);
	ft::Parser Parse(file, test);

	ft::HTTPServer	Serverlist(Parse.getHTTPServer());
	vector<struct pollfd> fds;
	char buf[3000];


	for (ft::HTTPServer::iterator it = Serverlist.begin(); it != Serverlist.end(); it++)
	{
		struct pollfd tmp;
		tmp.fd = it->second.getFd();
		tmp.events = POLLIN;
		fds.push_back(tmp);
	}

	// for (vector<struct pollfd>::iterator it = fds.begin(); fds)

	int server_size = Serverlist.size();
			bool connection = false;
	std::cout << " fds size is " << fds.size()  << std::endl;
	while (1)
	{

		if (poll(&fds[0], fds.size(), 100))
		{
			// std::cout << " fds revents is " <<  fds.data()->revents<< std::endl;
			for (int i = 0; i < fds.size(); i++)
			{
				if (fds[i].revents == 0)
					continue;
				if (i < server_size)
				{
					if (fds[i].revents & POLLIN)
					{
						std::cout << "connected" << std::endl;
						struct pollfd tmp;
						tmp.fd = Serverlist.findServer(fds[i].fd).getSocket().accept_connection();
						Serverlist.newClient(tmp.fd, fds[i].fd);
						tmp.events = POLLIN;
						fds.push_back(tmp);
					}
				}
				else if (fds[i].revents != 0)
				{
					// if (fds[i].fd)
					
					// std::cout << "STDfds[i].revents << 
			
					if (fds[i].revents & POLLIN)
					{
				
						int ret = read(fds[i].fd, buf, 3000);
						if (ret)
						{
							stringstream ss(buf);
							std::string str;
							ss >> str;
							std::cout<< str << std::endl;
							fds[i].events = POLLOUT;
						}
						else
							connection = true;
					}
					else if (fds[i].revents & POLLOUT)
					{
						std::cout << "SENDING" << std::endl;
						write(fds[i].fd, "HAHAHAHAHAHAHAAHAH", 19);
						fds[i].events = POLLIN;
						// poll_length--;
					}
					if (fds[i].revents & POLLHUP || connection == true)
					{
						Serverlist.deleteClient(fds[i].fd);
						std::cout << "Deleteing client fd : " << fds[i].fd << " that connnected to server fd : " << Serverlist.findServerfd(fds[i].fd) << std::endl;
						fds.erase(fds.begin() + i);
						connection = false;
					}
				}
			}
		}
	}
}

