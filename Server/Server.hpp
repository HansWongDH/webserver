#ifndef SERVER_HPP
# define SERVER_HPP

# include <unistd.h>
# include "Socket.hpp"

namespace ft
{
    class Server
    {
        public:
            Server() {}
            Server(int port): socket(port) {
                // client_fd = socket.accept_connection();
            }
            
            ~Server() {
                close(client_fd);
            }

            /**
             * @brief Keep waiting for a connection
             * 
             */
            
            int getFd(void)
            {
                return client_fd;
            }

            ft::Socket getSocket(void)
            {
                return this->socket;
            }

            void    bind_fd(int fd)
            {
                client_fd = fd;
            }

        private:
            ft::Socket socket;
            int client_fd;
    };
}

#endif