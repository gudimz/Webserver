#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "Server.hpp"
#include "parser.hpp"
#include "Location.hpp"
#include <iostream>
#include <vector>
#include <map>

namespace ft
{
    class Location;
    class Config {
    private:
        std::string                                 _host;
        std::string                                 _port;
        std::string                                 _server_name;
        std::string                                 _root;
        std::vector<std::string>                    _index;
        std::map<int, std::string>                  _error_pages;
        std::map<std::string, Location>             _locations;
        typedef std::vector<std::string>::iterator v_iter_string;
        typedef std::map<int, std::string>::iterator map_iter_int_string;
        typedef std::vector<std::string>::const_iterator v_const_iter_string;
        typedef std::vector<std::vector<std::string> >::iterator v_iter_v_string;
        typedef std::vector<std::vector<std::string> >::const_iterator v_const_iter_v_string;
        typedef std::vector<std::string> v_string;
        typedef std::vector<std::vector<std::string> > v_vec_string;
        typedef std::map<std::string, ft::Location>::iterator v_iterator_locationov;
    public:
        Config();
        Config(v_iter_v_string& it, v_vec_string& content);
        Config(const Config& other);
        ~Config();
        Config& operator=(const Config& other);

        v_iterator_locationov getBeginLocation();
        v_iterator_locationov getEndLocation();

        std::string const getHost(void) const;
        std::string const getPort(void) const;
        std::string const getServName(void) const;
        std::string const getRoot(void) const;
        std::vector<std::string> const getIndex(void) const;
        std::string const getErrPages(int key) const;
        std::map<std::string, ft::Location> getLocation(void) const;
        std::map<std::string, ft::Location>::iterator findKeyLocation(std::string key);

        void setHost(const v_string& line);
        void setPort(const v_string& line);
        void setPort(const std::string& port);
        void setServName(const v_string& line);
        void setRoot(const v_string& line);
        void setIndex(const v_string& line);
        void initErrPages(void);
        void setErrPages(const v_string& line);
        void setLocation(v_iter_v_string& it, v_vec_string& content, std::string server_root);
    };
}

#endif