#ifndef PARSER_HPP
#define PARSER_HPP

#include "Config.hpp"
#include "Server.hpp"
#include <iostream>
#include <algorithm>
#include <vector>

#define GREEN "\x1b[1;32m"
#define WHITE "\x1b[37m"
#define RED "\x1b[1;31m"
#define YEL "\x1b[1;33m"
#define MAG "\x1b[1;35m"
#define CYAN "\x1b[1;36m"
#define REST "\x1b[0m"

namespace ft
{
    class Config;
    typedef std::vector<std::string>::iterator v_iter_string;
    typedef std::vector<std::string>::const_iterator v_const_iter_string;
    typedef std::vector<std::vector<std::string> >::iterator v_iter_v_string;
    typedef std::vector<std::vector<std::string> >::const_iterator v_const_iter_v_string;
    typedef std::vector<std::string> v_string;
    typedef std::vector<std::vector<std::string> > v_vec_string;

	std::vector<ft::Config> parser(char* path);
    std::vector<ft::Config> parseServer(v_vec_string& content);
    void split(std::vector<std::string>& content, const std::string& line, char c);
    void printErrParser(const v_string& line,  std::string message);
    void checkContent(const v_vec_string&);
    int checkCountBrackets(const v_vec_string& content);
    void checkSemicolon(const v_vec_string& content);
    void checkPath(std::string path, std::string message);
    void checkValidPort(std::string& port);
    v_string checkMultiplePort(v_iter_v_string& it, const v_vec_string& content);
    void lineJoin(std::string& line);
    int readFile(v_vec_string& servers, char* path);
    std::vector<std::string> checkMultiplePort(v_vec_string& content);
	class ParserException : public std::exception {
    private:
        std::string _message;
    public:
        ParserException(std::string message) throw() ;
        const char* what(void) const throw();
        virtual ~ParserException() throw();
    };
}

#endif