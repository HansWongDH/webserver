#ifndef SERVER_HPP
# define SERVER_HPP

# include <unistd.h>
# include "Socket.hpp"
# include "Client.hpp"

namespace ft
{
    class Server
    {
        public:
            Server() {}
            Server(int port): socket(port), port(port) {

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

        private:
            ft::Socket socket;
            int port;
            std::map<int, ft::Client> m_client;
    };
}

#endif