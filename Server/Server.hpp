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
            ~Server() {}
            Server(const Server &copy);
            Server &operator=(const Server &copy);

            /**
             * @brief Keep waiting for a connection
             * 
             */
            void run()
            {
                const char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";

                while(1)
                {
                    printf("\n+++++++ Waiting for new connection ++++++++\n\n");
                    int new_socket = socket.accept_connection();
                    
                    char buffer[30000] = {0};
                    read(new_socket , buffer, 30000);
                    printf("%s\n",buffer );
                    write(new_socket , hello , strlen(hello));
                    printf("------------------Hello message sent-------------------\n");
                    close(new_socket);
                }
            }

        private:
            ft::Socket socket;
    };
}

#endif