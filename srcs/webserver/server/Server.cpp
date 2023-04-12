#include "Server.hpp"

ft::Server::Server() {}
ft::Server::Server(int port, ft::ServerInfo serv): socket(port), info(new ft::ServerInfo(serv)) {

}
            
ft::Server::~Server() {
    std::cout << RED << "server destructor called" << std::endl;
    delete info;
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

ft::ServerInfo  ft::Server::getInfo(void)
{
    return *this->info;
}

ft::ServerInfo* ft::Server::getInfoAddress(void)
{
    return this->info;
}