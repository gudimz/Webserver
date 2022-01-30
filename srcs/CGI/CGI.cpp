#include "CGI.hpp"

// Возвращает верхний регистр символа
char upperchar(char ch) {
	if ((ch >= 'a') && (ch <= 'z')) {
		ch = 'A' + (ch - 'a');
		 return ch;
	} else return ch;
};

std::string     toHex(int number)
{
    std::stringstream sstream;
    sstream << std::hex << number;
    std::string result = sstream.str();
    return result;
}

// Переводит из Hex в Dec
unsigned int    toDec(const std::string& number)
{
    unsigned int x;   
    std::stringstream ss;
    ss << number;
    ss >> std::hex >> x;
    return (x);
}

ft::CGI::CGI(ft::Response& req)
{
    std::cout << "=====================CGI======================\n";
    init_env(req);
    _argv = (char**)malloc(sizeof(char*) * 3);
    std::string cgi_path;
    std::vector<std::string> path_vec = req.current_location->getCgiPath();
    for (std::vector<std::string>::iterator it = path_vec.begin(); it != path_vec.end(); it++)
    {
        cgi_path = *it;
        if ((_env["SCRIPT_FILENAME"].find(".py") != std::string::npos && it->find("python") != std::string::npos) ||
            (_env["SCRIPT_FILENAME"].find(".php") != std::string::npos && it->find("php") != std::string::npos))
            break ;
    }
    _argv[0] = strdup(cgi_path.c_str());
    // _argv[0] = strdup((std::string(std::getenv("PWD")) + "/ubuntu_cgi_tester").c_str());
    _argv[1] = strdup(_env["SCRIPT_FILENAME"].c_str());
    _argv[2] = NULL;
}

ft::CGI::~CGI() { 
    int i = 0;
    for ( ; _cenv[i]; i++)
        free(_cenv[i]);
    free(_cenv[i]);
    free(_cenv);
    free(_argv[0]);
    free(_argv[1]);
    free(_argv[2]);
    free(_argv);
    _env.clear();
}
int     ft::CGI::countCookies(const std::string& str)
{
    int count = 1;
    int i = 0;
    while (str[i] != '\0')
        if (str[i++] == '&')
            count++;
    return (count);
}

std::string ft::CGI::setCookie(const std::string& str)
{
    std::string ret = str;
    size_t      pos;
    while ((pos = ret.find("&")) != std::string::npos)
    {
        ret.replace(pos, 1, ";\r\nSet-cookie:");
    }
    return (ret);
}

void    ft::CGI::formHeader(std::string& header)
{
    size_t dcrlf;
    std::string local;
    if (header.find("HTTP/1.1") == std::string::npos)
        header.insert(0, "HTTP/1.1 200 OK\r\n");
    if (header.find("Content-type:") == std::string::npos)
        local += "Content-type: text/html\r\n";
    local += "Transfer-Encoding: chunked\r\n";
    if (_env.count("HTTP_COOKIE") && header.find("Set-cookie") == std::string::npos)
        local += setCookie(_env["HTTP_COOKIE"]);
    if (header.find("Connection:") == std::string::npos)
        local += "Connection: keep-alive\r\n";
    if ((dcrlf = header.find("\r\n\r\n")) == std::string::npos)
    {
        local += "\r\n\r\n";
        dcrlf = header.find("\r\n") + 2;
    }
    else
        local.insert(0, "\r\n");
    header.insert(dcrlf, local);
}

std::string            ft::CGI::decode(std::string& path)
{
    size_t token = path.find("%");
    while (token != std::string::npos)
    {
        //replace token with ASCII
        //check if there are at least 2 symbols after %
        if (path.length() < token + 2)
            break ;
        char dec = toDec(path.substr(token + 1, 2));
        path.replace(token, 3, ft::to_string(dec));
        token = path.find("%");
    }
    return (path);
}
std::string     ft::CGI::getAfter(const std::string& path, char delim)
{
    size_t  res;
    res = path.find(delim);
    if (res == std::string::npos)
        return ("");
    else
        return (path.substr(res + 1, path.size()));
}

std::string            ft::CGI::getBefore(const std::string& path, char delim)
{
    size_t  res;
    res = path.find(delim);
    if (res == std::string::npos)
        return ("");
    else
        return (path.substr(0, res));
}

void            ft::CGI::init_env(ft::Response& req)
{
    _env["AUTH_TYPE"] = "Basic";
    _env["CONTENT_LENGTH"] = req.full_log["Content-Length"];
    _env["CONTENT_TYPE"] = req.full_log["Content-type"];
    _env["GATEWAY_INTERFACE"] = "CGI/1.1";
    _env["QUERY_STRING"] = req.full_log["Query_string"];
    _env["REMOTE_ADDR"] = req.full_log["Host"];
    _env["REQUEST_METHOD"] = req.full_log["ZAPROS"];
    _env["SCRIPT_FILENAME"] = req.current_location->getRoot() + req.full_log["Dirrectory"].substr(req.full_log["Dirrectory"].find_last_of('/') + 1);
    _env["PATH_INFO"] = decode(req.full_log["Path_info"]);
    _env["PATH_TRANSLATED"] = req.current_location->getRoot() + _env["PATH_INFO"] == "." ? "/" : _env["PATH_INFO"];
    _env["HTTP_COOKIE"] = req.full_log["Cookie"];
    _env["SERVER_NAME"] = getBefore(req.full_log["Host"], ':');
    _env["SERVER_PORT"] = getAfter(req.full_log["Host"], ':');
    _env["SERVER_PROTOCOL"] = "HTTP/1.1";
    _env["REDIRECT_STATUS"] = "200";

    _cenv = (char**)malloc(sizeof(char*) * (_env.size() + 1));
    int i = 0;
    for (std::map<std::string, std::string>::iterator it = _env.begin(); it != _env.end(); it++)
    {
        std::string concat;
        concat = it->first + "=" + it->second;
        _cenv[i] = strdup(concat.c_str());
        i++;
    }
    _cenv[i] = NULL;
    // _env.clear();
}

void             ft::CGI::execute(ft::Response& req, int fd, ft::Config& conf)
{
    pid_t   pid;
    int     pipe_in[2], pipe_out[2];
    int     status;

    if (pipe(pipe_in) < 0)
    {
        std::cerr << "pipe failed\n";
        req.answer(500, fd, conf);
        return ;
    }
    if (pipe(pipe_out) < 0)
    {
        std::cerr << "pipe failed\n";
        close(pipe_in[0]);
        close(pipe_in[1]);
        req.answer(500, fd, conf);
        return ;
    }
    pid = fork();
    if (pid == 0)
    {
        dup2(pipe_in[0], STDIN_FILENO);
        dup2(pipe_out[1], STDOUT_FILENO);
        write(pipe_in[1], req.full_log["Body"].c_str(), atoi(_env["CONTENT_LENGTH"].c_str()));
        close(pipe_in[0]);
        close(pipe_in[1]);
        close(pipe_out[0]);
        close(pipe_out[1]);
        status = execve(_argv[0], _argv, _cenv);
        req.answer(500, fd, conf);
        exit(status);
    }
    else if (pid > 0){

        close(pipe_in[1]);
        waitpid(pid, &status, 0);
        close(pipe_out[1]);
        if (status < 0)
        {
            close(pipe_out[0]);
            close(pipe_in[0]);
            return ;
        }

        char        buf[201];
        int         res = read(pipe_out[0], buf, 200);
        buf[res] = '\0';
        std::string header(buf);
        std::string body;
        size_t      dcrlf;

        formHeader(header);
        dcrlf = header.find("\r\n\r\n");
        if (dcrlf != std::string::npos){
            body = header.substr(dcrlf + 4);
            header.erase(dcrlf + 4);
        }
        send(fd, header.c_str(), header.length(), 0);
        std::cout << "======================HEADER======================\n";
        std::cout << header << std::endl;
        std::cout << "=======================BODY=======================\n";
        std::cout << body << std::endl;
        do 
        {
            std::string chunk;
            chunk = to_string(toHex(body.length()));
            chunk += "\r\n";
            chunk += body;
            chunk += "\r\n";
            std::cout << chunk << std::endl;
            send(fd, chunk.c_str(), chunk.length(), 0);
            std::cout << "Chunk sent\n";
            if (res <= 0)
                break;
            res = read(pipe_out[0], buf, 200);
            buf[res] = '\0';
            body = to_string(buf);
        }while (res > 0);
        send(fd, "0\r\n\r\n", 5, 0);
        close(pipe_out[0]);
        close(pipe_in[0]);
    }
    else
        std::cerr << "Fork failed\n";
}