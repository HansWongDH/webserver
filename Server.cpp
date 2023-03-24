# include "./Server/Server.hpp"
# include "./Parser/Parser.hpp"
# include <iostream>
# include <vector>
# include <poll.h>

using std::vector;
using std::cout;
using std::endl;

int	main(void)
{
	std::ifstream file;
	file.open("./config/example.conf");
	const char* temp[]	= {"listen", "server_name", "root"};
	std::vector<std::string> test (temp, temp + 3);
	ft::Parser Parse(file, test);

	ft::Parser::server_vector Serverlist(Parse.getServer());
	vector<struct pollfd> fds(Serverlist.size());
	char buf[3000];

int i = 0;
for (ft::Parser::server_iterator it = Serverlist.begin(); it != Serverlist.end(); it++, i++)
	{
		fds[i].fd = it->getSocket().getSocketfd();
		fds[i].events = POLLIN;
		fds[i].revents = 0;
	}

// for (vector<struct pollfd>::iterator it = fds.begin(); fds)

	int	server_size = Serverlist.size();
			std::cout << " fds revents is " <<  fds.data()->revents<< std::endl;
// while (1)
// {
		if (poll(&fds[0], fds.size(), 100))
		{
		
			std::cout << " fds revents is " <<  fds.data()->revents<< std::endl;
			for(int i = 0; i < fds.size(); i++)
			{
			if (i < server_size)
			{
				if(fds[i].revents == 0)
        			continue;
				if (fds[i].revents & POLLIN)
				{
					
					struct pollfd tmp;
					tmp.fd = Serverlist[i].getSocket().accept_connection();
					tmp.events = POLLIN;
					fds.push_back(tmp);
				}
			}
			else if (fds[i].revents != 0)
			{
				// if (fds[i].fd)
					// {
				if (fds[i].revents & POLLHUP)
				{
							// fds.erase(&fds[i]);
				}
				if (fds[i].revents & POLLIN)
				{
					std::cout<< "connected" << std::endl;
					read(fds[i].fd, buf, 3000);
					if (buf)
						fds[i].events = POLLOUT;
				}
				else if (fds[i].revents & POLLOUT)
				{
					std::cout << "SENDING" << std::endl;
					write(fds[i].fd, "HAHAHAHAHAHAHAAHAH", 19);
					fds[i].events = POLLIN;
					// poll_length--;
				}
				
				}
			}
				// }
			
		}
// }




	
}