#pragma once
#ifndef SOCKET_H__
#define SOCKET_H__

#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstring>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

namespace ft
{
    class Socket
    {
        private:
            int                 _sock;
            int                 _connection;
            struct sockaddr_in  _address;
            socklen_t           _addrlen;
            class SocketError : public std::exception
            {
                virtual const char*         what() const throw() { return ("Socket error"); }
            };
            class BadAddress : public std::exception
            {
                virtual const char*         what() const throw() { return ("Bad address"); }
            };
        public:
            Socket();
            struct sockaddr_in  getAddress();
            int                 getSock();
            int                 getConnection();
            socklen_t*          getAddrlen();
            int                 acceptSocket(sockaddr_in *addr, socklen_t *size);
            void                setConnection(int domain);
            void                listenSocket(std::string ip, std::string port);
    };
}

#endif