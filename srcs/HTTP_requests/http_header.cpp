#include "Response.hpp"

//GET, POST, DELETE
// application/x-www-form-urlencoded multipart/form-data

// 413 -- размере requeeset Больше лимита, приходит из конфига сервера
// 505 HTTP version not suported

int check_url(ft::Response& req, ft::Config& conf)
{
    std::string     real_root;
    std::string     real_dir;
    int             check_loc = 0;
    size_t          qs = req.full_log["Dirrectory"].find("?");

    if (qs != std::string::npos) // если есть query string
    {
        req.full_log["Query_string"] = req.full_log["Dirrectory"].substr(qs + 1, req.full_log["Dirrectory"].length());
        req.full_log["Dirrectory"].erase(qs, req.full_log["Dirrectory"].length());
    }
    else
        req.full_log["Query_string"] = "";
    req.full_log["Path_info"] = ".";
    std::map<std::string, ft::Location>::iterator it = conf.getBeginLocation();
    while(it != conf.getEndLocation())
    {
        int stop = 0;
        if (req.full_log["Dirrectory"].find(it->first) != std::string::npos)
        {
            for (size_t i = 0; i < it->second.getCgiExtension().size(); i++)
            {
                std::string cgi_extension = it->second.getCgiExtension()[i];
                if (cgi_extension != "")
                {
                    qs = req.full_log["Dirrectory"].find(cgi_extension);
                    if (qs != std::string::npos)
                    {
                        if (req.full_log["Dirrectory"].substr(qs + cgi_extension.length())[0] != '/')
                        {
                            stop = 1;
                            break ;
                        }
                        req.full_log["Path_info"] = req.full_log["Dirrectory"].substr(qs + cgi_extension.length());
                        req.full_log["Dirrectory"].erase(qs + cgi_extension.length());
                        if (req.full_log["Path_info"] == "/" || req.full_log["Path_info"] == "")
                            req.full_log["Path_info"] = ".";
                        stop = 1;
                        break ;
                    }
                }

            }
        }
        if (stop)
            break ;
        it++;
    }
    it = conf.getBeginLocation();

    while(it != conf.getEndLocation()) // проверяем все Location'ы из конфиг файла
    {   
        real_dir = req.full_log["Dirrectory"].substr(req.full_log["Dirrectory"].length() > 1 ? 1 : 0, req.full_log["Dirrectory"].length());
        check_loc = real_dir.find((*it).first);
        if((!check_loc && (*it).second.getAutoindex()) || (!check_loc && (check_loc + real_dir.length() == (*it).first.length())))
            check_loc = 1;
        else
            check_loc = 0;
        if((check_loc && (*it).second.getAutoindex()) || (check_loc && !req.full_log["ZAPROS"].compare(0,6,"DELETE")))
            req.full_log["Auto-Index"] = req.full_log["Dirrectory"].substr((*it).first.length() + 1, req.full_log["Dirrectory"].length());
        if(check_loc || ((*it).second.getAutoindex() && check_loc)) // если обращение пришло по Location или автоиндекс
        {
            req.current_location = &(*it).second;
            if(!req.current_location->getCgiExtension().empty() && !((*it).second.getAutoindex()))
                return 404;
            return 0;
        }
        else // обратились на индексовый файл в Location'е который прописан, например localhost:8080/index.html
        {
            real_root = (*it).second.getRoot().substr(0, (*it).second.getRoot().size() - 1);
            size_t i = 0;
            int n = req.full_log["Dirrectory"].find_last_of("/");
            if(!req.full_log["Dirrectory"].substr((n == 0) ? 0 : 1, (n == 0) ? 1 : n).compare((*it).first))
            {
                while (i < (*it).second.getIndex().size())
                {
                    if((real_root + req.full_log["Dirrectory"].substr(n, req.full_log["Dirrectory"].size())) == (*it).second.getIndex()[i])
                    {
                        req.current_location = &(*it).second;
                        return 0;
                    }
                    i++;
                }
            }
        }
        it++;
    }

    return 404;
}

bool http_header(ft::Response& req, std::string buf1, int fd, ft::Config& conf)
{
    std::string buffer;
    std::istringstream is(buf1);

    while(std::getline(is, buffer, '\n')) // парс хэдеров, ничего тут не предстоит делать, если не понадобится какой-то особенный хэдер ( особенные лежат в дефолт конструкторе Responsee - добавь если понадобится + продублируй в clear())
    {
        if(!buffer.compare(0, 5, "Host:"))
        {
            if(req.full_log["Host"] == "")
                req.full_log["Host"]= buffer.substr((buffer[5] == ' ') ? 6 : 5);
            req.full_log["Host"].erase(req.full_log["Host"].begin() + (req.full_log["Host"].find('\r')));
        }
        else if(!buffer.compare(0, 11, "Connection:"))
        {
            req.full_log["Connection"] = buffer.substr((buffer[11] == ' ') ?  12 : 11);
            if(req.full_log["Connection"].compare(0, 11, "Keep-Alive\r") && req.full_log["Connection"].compare(0, 6, "close\r"))
                req.full_log["Connection"] = "Keep-Alive";
        }
        else if(!buffer.compare(0, 18, "Transfer-Encoding:"))
        {
            if(req.full_log["Transfer-Encoding:"] == "")
                req.full_log["Transfer-Encoding:"] = buffer.substr((buffer[18] == ' ') ?  19 : 18);
            if(req.full_log["Transfer-Encoding"].compare(0, 7,"chunked"))
                req.is_chunked = true;
        }
        else if(!buffer.compare(0, 13, "Content-Type:") || !buffer.compare(0, 13, "Content-type:"))
        {
            if(req.full_log["Content-Type"] == "")
            {
                size_t i;
                req.full_log["Content-Type"] = buffer.substr((buffer[13] == ' ') ?  14 : 13);
                if(req.full_log["Content-Type"].find("multipart/form-data") != std::string::npos)
                {
                    i = req.full_log["Content-Type"].find("boundary=", 0);
                    if(i != std::string::npos)
                    {
                        req.full_log["boundary"] = req.full_log["Content-Type"].substr(i+9, req.full_log["Content-Type"].size());
                        req.full_log["boundary"].erase(req.full_log["boundary"].begin() + req.full_log["boundary"].find('\r'));
                    }
                    else
                        return req.answer(400,fd,conf);
                    req.is_multy = true;
                }
            }
        }
        else if(!buffer.compare(0, 15, "Content-length:") || !buffer.compare(0, 15, "Content-Length:"))
        {
            req.is_content_length = true;
            if(req.full_log["Content-Length"] == "")
                req.full_log["Content-Length"] = buffer.substr((buffer[15] == ' ') ?  16 : 15);
        }
        else if(!buffer.compare(0, 6, "Range:"))
        {
            req.full_log["Range"] = buffer.substr(buffer.find_first_of('=')+1, buffer.size() - 1);
            if( req.full_log["Range"][0] == '-')
                req.range_begin = 0;
            else
                req.range_begin = ft::ft_atoi(req.full_log["Range"]);
            std::string temp = req.full_log["Range"].substr(req.full_log["Range"].find_first_of('-'), req.full_log["Range"].size());
        }
        else if(!buffer.compare(0,7, "Cookie:"))
            req.full_log["Cookie"] = buffer.substr(8, buffer.size() - 1);

        if(!buffer.compare(0, 1, "\r")) // кончились хедеры - тело записывается в CLuster.cpp
            break;   
    }
    if(!req.full_log["Host"].size()) 
    {
        return(req.answer(400, fd, conf));
    }
    if (req.full_log["Host"].find(conf.getHost()) != std::string::npos && req.full_log["Host"].find(conf.getPort()) != std::string::npos) 
    {
        if (conf.getHost().size() + conf.getPort().size() + 1 != req.full_log["Host"].size()) 
        {
            return(req.answer(400, fd, conf));
        }
    } else if (req.full_log["Host"].find(conf.getServName()) != std::string::npos && req.full_log["Host"].find(conf.getPort()) != std::string::npos){
        if (conf.getServName().size() + conf.getPort().size() + 1 != req.full_log["Host"].size()) {
            return(req.answer(400, fd, conf));
        }
    } else {
        return(req.answer(400, fd, conf));
    }
    int i = check_url(req, conf);
    if(i) // вот тут происходит чек location
        return(req.answer(i,fd, conf));
    i =  req.req_methods_settings((req.current_location->getMethods())); // вот здесь спец-настройка в замисимости от метода и хэдеров
    if(i)
    {
        return(req.answer(i, fd, conf));
    }
    return true;
}