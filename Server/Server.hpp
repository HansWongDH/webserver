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
            Server(int port): socket(port) {}
            
            ~Server() {
                close(client_fd);
            }

            /**
             * @brief Keep waiting for a connection
             * 
             */
            void run()
            {
                const char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 18\n\nChicken pie world!";

                printf("\n+++++++ Waiting for new connection ++++++++\n\n");
                client_fd = socket.accept_connection();

                printf("Accepted\n");
                char buffer[30000] = {0};
                read(client_fd , buffer, 30000);
                printf("%s\n",buffer );
                write(client_fd , hello , strlen(hello));
                printf("------------------Hello message sent-------------------\n");
                close(client_fd);
            }

        private:
            ft::Socket socket;
            int client_fd;
    };
}

#endif