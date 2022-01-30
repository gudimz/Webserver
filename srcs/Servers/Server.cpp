#include "Server.hpp"

#define NUM_FDS 10

ft::Server::Server(std::string ip, std::string port)
{
    try
    {
        _socket.listenSocket(ip, port);
        _server = _socket.getSock();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what()<<std::endl;
		exit(1);
    }
}

int ft::Server::newConnection()
{
    sockaddr_in addr = _socket.getAddress();
	socklen_t size = sizeof(sockaddr_in);
	try {
	    int sock =_socket.acceptSocket(&addr, &size);
        fcntl(sock, F_SETFL, O_NONBLOCK);
        // _client.push_back(Client(sock));
        return (sock);
	}catch (std::exception &e){
		std::cerr << e.what()<<std::endl;
	}
    return (-1);
}

int            ft::Server::getServer() const
{
    return (_server);
}

ft::Socket      ft::Server::getSocket()
{
    return (_socket);
}