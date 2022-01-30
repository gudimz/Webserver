#pragma once
#ifndef CLUSTER_H__
#define CLUSTER_H__

#include "Server.hpp"

namespace ft
{
    class Cluster
    {
        private:
            std::vector<ft::Server>     _servers;
            std::vector<ft::Config>     _configs;
            struct pollfd               *_connected;
            int                         _size;
            int                         _capacity;
        public:
            Cluster();
            Cluster(const Cluster& other);
            ~Cluster();
            
            Cluster&                    operator=(const Cluster& other);

            void                        push_back(const ft::Server& server);
            void                        erase_poll(int index);
            void                        push_poll(int fd);
            void                        setup();
            void                        setConfig(std::vector<ft::Config> configs);
            void                        run();
            int                         receive(int fd, std::map<size_t, ft::Response>& all_connection, ft::Config& config);
            int                         is_listening(int fd);

            class ProcessError : public std::exception
            {
                virtual const char*         what() const throw() { return ("Process error"); }
            };
    };
}

#endif