#ifndef SERVER_HPP
# define SERVER_HPP

# include <unistd.h>
# include <map>
# include "Socket.hpp"
# include "../Parser/Serverblock.hpp"
# include "../Server/Client.hpp"
namespace ft
{
    class Server
    {
        public:
            Server() {}
            Server(int port, ft::ServerBlock serv): socket(port), server(serv) {
                // client_fd = socket.accept_connection();
            }
            
            ~Server() {
                // close(client_fd);
            }

            /**
             * @brief Keep waiting for a connection
             * 
             */
            
            // int getFd(void)
            // {
            //     return client_fd;
            // }

            ft::Socket getSocket(void)
            {
                return this->socket;
            }

            int getFd(void)
            {
                return this->socket.getSocketfd();
            }

  

            void	insertClient(int server_id, int client_fd)
			{
				ft::Client tmp(server_id);
				
				client.insert(std::make_pair(client_fd, tmp));
			}

			void	eraseClient(int client_fd)
			{
				client.erase(client_fd);
			}
            // void    bind_fd(int fd)
            // {
            //     client_fd = fd;
            // }

        private:
            ft::ServerBlock server;
            ft::Socket socket;
            std::map<int, ft::Client> client;


    };
}

#endif