#ifndef SERVER_H__
#define SERVER_H__

#include "Config.hpp"
#include "parser.hpp"
#include "Socket.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <poll.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/select.h>
#include <vector>
#include "Response.hpp"
#include "CGI.hpp"

namespace ft
{
    class Server
    {
        private:
            ft::Socket                              _socket;
            int                                     _server;
        public:
            Server(std::string ip, std::string port);
            int                             newConnection();
            void                            closeConnection(int client_index);
            int                       getServer() const;
            std::vector<pollfd>             getPollFds();
            ft::Socket                      getSocket();

    };
}


#endif