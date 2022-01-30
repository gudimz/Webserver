#pragma once
#ifndef CGI_H__
#define CGI_H__

// #include "Server.hpp"
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <map>
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "Server.hpp"

namespace ft
{
    struct Response;
    class CGI
    {
        private:
            std::map<std::string, std::string>  _env;
            char**                              _cenv;
            std::string                         _data;
            char**                              _argv;
            std::string                         _path;
            std::string                         getAfter(const std::string& path, char delim);
            std::string                         getBefore(const std::string& path, char delim);
            std::string                         decode(std::string& path);
            std::string                         setCookie(const std::string& str);
            int                                 countCookies(const std::string& str);

        public:
            CGI(ft::Response& req);
            ~CGI();
            void                                formHeader(std::string& header);
            void                                init_env(ft::Response& req);
            void                                CGI_read(long fd);
            void                                execute(ft::Response& req, int fd, ft::Config& conf);

    };
}

#endif 