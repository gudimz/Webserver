#include "Response.hpp"
ft::Response::~Response() {}
ft::Response::Response(ft::Response const& other)
{
    *this = other;
}
ft::Response& ft::Response::operator=(ft::Response const& other)
{
    if(this != &other)
    {
        this->full_log = other.full_log;
        this->full_buffer = other.full_buffer;
        this->is_content_length = other.is_content_length;
        this->is_multy = other.is_multy;
        this->is_chunked = other.is_chunked;
        this->is_delete = other.is_delete;
        this->body_length = other.body_length;
        this->is_body_left = other.is_body_left;
        this->is_file_large = other.is_file_large;
        this->path_large_file = other.path_large_file;
        this->current_location = other.current_location;
        this->body.str(other.body.str());
        this->file_size = other.file_size;
        this->range_begin = other.range_begin;
        this->is_redir = other.is_redir;
        this->is_dowland = other.is_dowland;
        this->download_error = other.download_error;
    }
    return *this;
}
ft::Response::Response()
{
    this->is_redir = false;
    this->full_log["Connection"] = "Keep-Alive";
    this->current_location = NULL;
	this->is_body_left = false;
    this->is_content_length = false;
    this->is_chunked = false;
    this->is_multy = false;
    this->is_delete = false;
    this->body_length = 0;
    this->is_file_large = false;
    this->file_size = 0;
    this->range_begin = 0;
    this->is_dowland = false;
    this->download_error = 0;
}


void ft::Response::clear()
{
    this->is_redir = false;
    this->full_log.clear();
    this->full_log["Connection"] = "Keep-Alive";
    this->current_location = NULL;
    this->body_length = 0;
    this->is_content_length = false;
    this->is_chunked = false;
    this->is_delete = false;
    this->is_multy = false;
    this->is_file_large = false;
    this->path_large_file.clear();
    input.close();
    this->file_size = 0;
    this->range_begin = 0;
    this->is_dowland = false;
    this->download_error = 0;
    this->dowland_body.clear();
}
bool ft::Response::AutoIndexPage(ft::Config& conf)
{   
    std::string dir_name = current_location->getRoot() + this->full_log["Auto-Index"];
    std::string dir_index = full_log["Dirrectory"].substr(1, full_log["Dirrectory"].length() - 1);
    std::string req;
    DIR *dir = opendir(dir_name.c_str());
    struct dirent *ent;
    struct stat dir_check;
    if(!stat(dir_name.c_str(), &dir_check))
    {
        if(S_ISREG(dir_check.st_mode))
        {
            this->full_log["Content-Type"] = "application/octet-stream";
            if(this->full_log["Dirrectory"].find(".png") != std::string::npos)
                this->full_log["Content-Type"] = "image/png";
            else if(this->full_log["Dirrectory"].find(".jpg") != std::string::npos)
                this->full_log["Content-Type"] = "image/jpg";
            else if(this->full_log["Dirrectory"].find(".jpeg") != std::string::npos)
                this->full_log["Content-Type"] = "image/jpeg";
            else if(this->full_log["Dirrectory"].find(".gif") != std::string::npos)
                this->full_log["Content-Type"] = "image/gif";
            else if(this->full_log["Dirrectory"].find(".mp4") != std::string::npos)
                this->full_log["Content-Type"] = "video/mp4";
            else if(this->full_log["Dirrectory"].find(".html") != std::string::npos)
                this->full_log["Content-Type"] = "text/html";
            input.open(dir_name.c_str(), std::ios::binary|std::ios::in);
	    	if(!input.is_open())
	    		return false;
            input.seekg(0, std::ios::end);
            file_size = input.tellg();
            input.seekg(0, std::ios::beg);
            if(range_begin && range_begin < file_size && range_begin > 0)
                input.seekg(range_begin);
            if(file_size < 100000000)
            {
                body << input.rdbuf();
                input.close();
            }
            else
                is_file_large = true;
            return true;
        }
    }
    if(!dir)
    {
        std::cout << "Cant open dirr" << std::endl;
        body.str("");
		return false;
    }
    else
    {
        if(*(full_log["Dirrectory"].end()-1) != '/')
        {
            full_log["Dirrectory"] += "/";
            this->is_redir = true;
        }
    }
    req = "<!DOCTYPE html>\r\n<html>\r\n<head>\r\n<meta http-equiv=\"Content-Type\" content=\"text/html\">\r\n<title>" +this->full_log["Dirrectory"]+"</title>\r\n</head>\r\n<body>\r\n<h1> Index of " + full_log["Dirrectory"] +"</h1>\r\n<hr>";
    while ((ent=readdir(dir)) != NULL) {
        if(!strcmp(".", ent->d_name))
            continue;
		if(strstr(ent->d_name, ".html") != NULL)
		{
			input.open((dir_name + ent->d_name).c_str());
			body << input.rdbuf();
		    closedir(dir);
			return true;
		}
        req +="\r\n<p><a href=\"http://" + conf.getHost();
        req += ":";
        req += conf.getPort();
        if(strstr("..", ent->d_name) && full_log["Auto-Index"].empty())
            req += full_log["Dirrectory"];
        else
        {
            req += "/";
            req += dir_index;
            req += ent->d_name;
            std::string str = dir_name;
            str += ent->d_name;
            if(!stat(str.c_str(), &dir_check))
            {
                if(S_ISDIR(dir_check.st_mode)){req += "/";}
            }// проверка на дирректорию, если она = добавляем / в конце
        }
        req += "\">";
        req += ent->d_name;
        req += "</a></p>";
    }
    req += "\r\n<hr></body>";
    closedir(dir);
    body.str(req);
    return true;
}

bool ft::Response::answer(int i, int fd, ft::Config& conf)
{
    time_t now = time(0);
    std::string time = ctime(&now);
    std::string head;
    bool ans = false;
    if(i == 200)
    {
        this->full_log["Content-Type"] = "text/html";
        this->full_log["Location"] =  "http://"+this->full_log["Host"]+this->full_log["Dirrectory"];
        if((current_location->getAutoindex()))
        {
			this->body.str("");
			this->body.str().clear();
            if(!this->AutoIndexPage(conf))
                return (answer(400, fd, conf));
            this->full_log["Location"] =  "http://"+this->full_log["Host"]+this->full_log["Dirrectory"];
        }
        else // если не автоиндекс
        {
            size_t i = 0;
            while(i < current_location->getIndex().size()) // ищем страничку с индексом и открываем ее
            {
                std::ifstream input (current_location->getIndex()[i].c_str());// проверять, если буфер == 0, то попробовать следующий, выкинуть 403
                if(input.is_open())
                {
					body.str("");
					body.str().clear();
					body.clear();
                    body << input.rdbuf();
					input.close();
                    break;
                }
                i++;
            }
            if(i == current_location->getIndex().size()) // если такого индекса не оказалось в папке
                return this->answer(403,fd,conf);
        }
        if(this->is_redir)
                return this->answer(301,fd,conf);
        if(full_log["Range"].size())
            head = "HTTP/1.1 206 " + status(206);
        else
		    head = "HTTP/1.1 200 " + status(200);
		head += "\r\nLocation: " +this->full_log["Location"];
		head += "\r\nContent-Type: ";
		head += this->full_log["Content-Type"];
        if(full_log["Range"].size())
            head += "\r\nContent-Range: bytes "+ ft::to_string(range_begin)+ "-" + ft::to_string(file_size - 1) +"/"+ft::to_string(file_size);
		else
            head += "\r\nAccept-Ranges: bytes";
        head +="\r\nDate: "\
        +time+"Server: WebServer/1.0\r\nContent-Length: ";
        if(is_file_large)
            head += ft::to_string(file_size - range_begin);
        else
            head += (ft::to_string(body.str().size()));
        head += "\r\nConnection: " +this->full_log["Connection"]; //+"\r\n";
        head += "\r\n\r\n";
        std::cout << head;
		head += body.str();
		is_body_left = true;
		body.str(head);
        return true;
    }
    else if(i == 301)
    {
		this->full_log["Location"] =  "http://"+this->full_log["Host"]+this->full_log["Dirrectory"];
        head = "HTTP/1.1 301 " + status(301) + "\r\nDate: "+time+"Content-Type: text/html\r\nContent-Length: 190\r\nAllow: GET, POST" + "\r\nConnection: "\
        +this->full_log["Connection"]+"\r\nServer: WebServer/1.0\r\nLocation: " +this->full_log["Location"]+"\r\n\r\n<html>\r\n<head><title>301 Moved Permanently</title></head>\r\n<body>\r\n<center><h1>301 Moved Permanently</h1></center>\r\n<hr><center>WebServer/1.0 (Ubuntu recomended)</center>\r\n</body>\r\n</html>\r\n";
        std::cout << head << std::endl;
        send(fd, head.c_str(), head.size(), 0);
        this->is_redir = false;
        ans = (full_log["Connection"].compare(0, 5, "close") ? true : false);
    }
    else if(i == 204)
    {
        head = "HTTP/1.1 204 " + status(204) + "\r\nDate: "+time+"\r\n\r\n";
        send(fd, head.c_str(), head.size(), 0);
        ans = (full_log["Connection"].compare(0, 5, "close") ? true : false);
    }
    else if(i == 500)
    {
        head = "HTTP/1.1 " + ft::to_string(i) +" "+ status(i) + "\r\nDate: "+time+"Content-Type: text/html\r\nContent-Length: 0\r\nConnection: close\r\n\r\n";
        std::cout << head << std::endl;
        send(fd, head.c_str(), head.size(), 0);
    }
    else 
    {
        std::ifstream input (conf.getErrPages(i).c_str());
        body << input.rdbuf();
        head = "HTTP/1.1 " + ft::to_string(i) +" "+ status(i) + "\r\nDate: "+time+"Content-Type: text/html\r\nContent-Length: "+(ft::to_string(body.str().size()))+"\r\nConnection: close\r\n\r\n";
        std::cout << head << std::endl;
        head += body.str();
        send(fd, head.c_str(), head.size(), 0);
    }
    clear();
    full_buffer.clear();
    body.str("");
    body.clear();
    body.str().clear();
    return (ans);
}

void* ft::Response::post_download_request()
{
    std::string buffer;
    std::string filename;
    bool is_bound = false;
    bool is_body = false;
    std::vector<std::string> for_filename;
    size_t i = 0;
    size_t for_split = 0;
    if(this->full_log["Body"].find("--"+this->full_log["boundary"]) != std::string::npos &&  this->full_log["Body"].find("--"+this->full_log["boundary"]+"--") != std::string::npos)
    {
        while(for_split < static_cast<size_t>(body_length))
        {
            buffer.clear();
            while(this->full_log["Body"][for_split] != '\n')
            {
                buffer += this->full_log["Body"][for_split];
                for_split++;
            }
            if(!buffer.compare(("--"+this->full_log["boundary"])+"--\r"))
            {
                is_body = true;
                is_bound = false;
            }
            if(!buffer.compare(("--"+this->full_log["boundary"]+'\r')))
                is_bound = true;
            if(is_bound)
            {
                if(!buffer.compare(0, 31, "Content-Disposition: form-data;"))
                {
                    ft_split(buffer, ';', for_filename);
                    std::vector<std::string>::iterator it = for_filename.begin();
                    while (it != for_filename.end())
                    {
                        i = (*it).find("filename=");
                        if(i != std::string::npos)
                        {
                            (*it).erase((*it).begin() + (*it).find('\r'));  
                            filename = (*it).substr(i + 9, (*it).size());
                            while ((i = filename.find('"')) != std::string::npos)
                            {
                                filename.erase(filename.begin() + i);
                            }
                        }
                        it++;
                    }   
                }
                else if(!buffer.compare(0, 1, "\r") && !filename.empty())
                {
                    is_bound = false;
                    is_body = true;
                }
            }
            else if(is_body)
            {
                if(!buffer.compare(("--"+this->full_log["boundary"]+'\r')))
                {
                    is_bound = true;
                }
                else if(!buffer.compare(("--"+this->full_log["boundary"])+"--\r"))
                {
                    path_large_file = "downloads/"+ filename;
                    is_dowland = true;
                    is_body_left = true;
                    dowland_body.erase(dowland_body.end() - 1);
                    body.str(dowland_body);
                    break;
                }
                else
                    dowland_body += (buffer + "\n");
            }
        for_split++;
        }
        return 0;
    }
    download_error = 400;
    return 0;
}

bool ft::Response::general_header_check(std::string str, int fd, ft::Config& conf)
{
    std::vector<std::string> header;
    size_t i = 0;
    if(!this->full_log["ZAPROS"].size())
    {
        ft_split(str, ' ', header);
        if(header.size() < 3)
        {
            answer(400, fd, conf);
            this->full_buffer.clear();
            this->full_log.clear();
            return false;
        }
        if(!header[0].compare(0, 3, "GET") && header[0].length() == 3)
            this->full_log["ZAPROS"] = header[0];
        else if(!header[0].compare(0,4,"POST") && header[0].length() == 4)
            this->full_log["ZAPROS"] = header[0];
        else if(!header[0].compare(0,6,"DELETE") && header[0].length() == 6)
            this->full_log["ZAPROS"] = header[0];
        else
            i = 405;
        if(header[1][0] != '/')
            i = 400;
        if(header[2].compare(0, 8, "HTTP/1.0") && header[2].compare(0, 8, "HTTP/1.1"))
            i = 505;
        this->full_log["Dirrectory"] = header[1];
    }
    if(i > 0)
    {
        answer(i, fd, conf);
        this->full_buffer.clear();
        this->full_log.clear();
        return(false);
    }
    return true;

}

int ft::Response::req_methods_settings(std::vector<std::string> str)
{
    std::vector<std::string>::iterator it = str.begin();
    std::string methods;
    while (it != str.end())
    {
        methods += *it;
        it++;
    }
    if(!this->full_log["ZAPROS"].compare(0, 3, "GET")) // если запрос GET - почти на все насрать
    {
        this->is_content_length = false;
        this->is_chunked = false;
        this->is_multy = false;
    }
    if(!this->full_log["ZAPROS"].compare(0,4,"POST")) // если пост
    {
        if(methods.find("POST") == std::string::npos) // если не разрешен метод = ошибка
            return(405);
        if(this->full_log["Content-Type"].empty()) // если нет Content-type - Ошибка
            return(400);
        this->body_length =  ft::ft_atoi(this->full_log["Content-Length"]); // полуучаем длинну контента
        if(!this->body_length) // если == 0, то убираем флаг на длинну контента
        {
            this->is_content_length = false;
            return (411);
        }
        if(!this->is_chunked && !current_location->getMaxBody().empty() && (ft::ft_atoi(current_location->getMaxBody()) < static_cast<long>(this->body_length)))
            return (413);
        if(this->is_chunked && !this->is_multy) // если есть Transfer-Encoding: chunked, то длинна контента игнорируется
        {
            this->is_content_length = false;
            this->body_length = 0;
        }
        if(this->is_multy) // если тип данных multipart/form-data - надо убарть chunked (по RFC)
            this->is_chunked = false;
    }
    if(!this->full_log["ZAPROS"].compare(0,6,"DELETE")) // если delete
    {
        if(methods.find("DELETE") == std::string::npos) // если не разрешен метод = ошибка
            return(405);
        this->is_delete = true;
        this->is_content_length = false;
        this->is_chunked = false;
        this->is_multy = false;
    }
    return 0;
}

std::string ft::Response::status(int code) {
    std::map<int, std::string> status;
    status[100] = "Continue"; //"Продолжить". Этот промежуточный ответ указывает, что запрос успешно принят и клиент может
                              // продолжать присылать запросы либо проигнорировать этот ответ, если запрос был завершён.
    status[200] = "OK"; // "Успешно". Запрос успешно обработан. Что значит "успешно", зависит от метода HTTP
    status[202] = "Accepted"; // "Принято". Запрос принят, но ещё не обработан. Это предназначено для случаев,
                             // когда запрос обрабатывается другим процессом или сервером, либо для пакетной обработки.
	status[206] = "Partial Content";
    status[204] = "No Content"; // "Нет содержимого". Нет содержимого для ответа на запрос, например при методе DELETE
    status[301] = "Moved Permanently"; // "Перемещён на постоянной основе". Этот код ответа значит,
                                       // что URI запрашиваемого ресурса был изменён. Возможно, новый URI будет предоставлен в ответе.
    status[307] = "Temporary Redirect"; // "Временное перенаправление". Сервер отправил этот ответ, чтобы клиент получил
                                       // запрошенный ресурс на другой URL-адрес с тем же методом, который использовал предыдущий запрос.
    status[400] = "Bad Request"; // "Плохой запрос". Этот ответ означает, что сервер не понимает запрос из-за неверного синтаксиса.
    status[403] = "Forbidden"; // "Запрещено". У клиента нет прав доступа к содержимому, поэтому сервер отказывается дать надлежащий ответ.
    status[404] = "Not Found"; // "Не найден". Сервер не может найти запрашиваемый ресурс.
    status[405] = "Method Not Allowed"; // "Метод не разрешён". Метод не может быть использован, потому что не указан в конфиге. Методы GET и HEAD всегда разрешены
    status[408] = "Request Timeout"; // Он означает, что сервер хотел бы отключить это неиспользуемое соединение
    status[413] = "Payload Too Large "; // Размер запроса превышает лимит, объявленный сервером. Сервер может закрыть соединение, вернув заголовок Retry-After
    status[500] = "Internal Server Error"; // "Внутренняя ошибка сервера". Сервер столкнулся с ситуацией, которую он не знает как обработать.
    status[501] = "Not Implemented"; // "Не выполнено". Метод запроса не поддерживается сервером и не может быть обработан. Исключение GET и HEAD
    status[505] = "HTTP Version Not Supported"; // "HTTP-версия не поддерживается". HTTP-версия, используемая в запросе, не поддерживается сервером.
    return status[code];
}

void ft_split(std::string const &str, const char delim,
            std::vector<std::string> &out)
{
    size_t start;
    size_t end = 0;
 
    while ((start = str.find_first_not_of(delim, end)) != std::string::npos)
    {
        end = str.find(delim, start);
        out.push_back(str.substr(start, end - start));
    }
}

size_t  ft_hex_to_dec(std::string& str)
{
    size_t x;   
    std::stringstream ss;
    ss << std::hex << str;
    ss >> x;
    return(x);
}