#include "Server.hpp"

ft::Server::Server() {}
ft::Server::Server(int port, ft::ServerInfo serv): socket(port) {
    this->info = new ft::ServerInfo(serv);
}
            
ft::Server::~Server() {
    // close(client_fd);
}

ft::Server::Server(const Server& other)
{
    this->socket = other.socket;
    this->info = other.info;
}

ft::Socket ft::Server::getSocket(void)
{
    return this->socket;
}

int ft::Server::getFd(void)
{
    return this->socket.getFd();
}

ft::ServerInfo*  ft::Server::getInfo(void)
{
    return this->info;
}