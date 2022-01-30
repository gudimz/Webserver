#pragma once
#ifndef RESPONSE_HPP
#define RESPONSE_HPP
#include <map>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include "Server.hpp"
#include <ctime>
#include <sstream>
#include "Config.hpp"
#include <dirent.h>
#include "Location.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
namespace ft
{
    class Location;
    class Config;
    struct Response
    {
        std::string full_buffer; // это полный текст всех хэдеров, он меняется очень много раз
        std::map<std::string, std::string> full_log; // фул лог, тут хэдеры
        bool is_content_length; // если есть Content-length и нет chunked
        bool is_chunked; // Content-length: chunked
        bool is_multy; // Content-type: multipary/*
        bool is_redir; // если надо ответить 301
        bool is_delete; // Если метод DELETE
        bool is_dowland; // если POST на загрузку
        long body_length; // если есть Content-length в запросе и ОТСУТСВУЕТ chunked (is_chunked = false). При чанкеде вручную body-length взять надо будет, this->full_log["Body"].size();
        bool is_body_left; // если сформирован body при 200 ответе
        bool is_file_large; // файл слишком большой
        std::string path_large_file; // путь до файла большого
		Location* current_location; // текущий Location из файла
        std::ifstream      input; // для больших файлов читать 
    	std::ostringstream body;  // body ответа
        long file_size; // полный размер большшого файла
        long range_begin; // если пришел запрос с Accept-range: от - до, в основном все присылается от - до конца
        //поток 
        int download_error;
        std::string dowland_body;
        // методы
        Response();
        ~Response();
        Response(Response const& other);
        Response& operator=(Response const& other); // конец Coplien формы
        void clear(); // очищает хедеры запроса текущего
        bool answer(int i, int fd,  ft::Config& conf); // тут куются ответы
        bool general_header_check(std::string str, int fd, ft::Config& conf); // проверка главного хэдера
        int req_methods_settings(std::vector<std::string> str); // проверка на то, какой метод пришел и что я могу с этим сделать
        bool AutoIndexPage(ft::Config& conf); // автоиндекс
        void* post_download_request(); // загрузка на сервер
        std::string status(int code); // в аргумент передается код ошибки, возвращается название ошибки
    };
    template<typename T>
    std::string to_string(const T& value) // что либо в строку
    {
    	std::ostringstream oss;
    	oss << value;
    	return oss.str();
    }
    template<typename T>
    long ft_atoi(T& str) // строку в число
    {
        long i = 0;
        std::stringstream ss;
        ss << str;
        ss >> i;
        return i;
    }
}
bool    http_header(ft::Response& req, std::string buf1, int fd,  ft::Config& conf); // парс хедеров
void    ft_split(std::string const &str, const char delim, std::vector<std::string> &out); // разделение по символу слов
size_t  ft_hex_to_dec(std::string& str); // 16 -> 10
#endif