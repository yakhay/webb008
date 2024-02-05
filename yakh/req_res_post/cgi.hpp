#ifndef CGI_HPP
#define CGI_HPP

#include "../config_file/configFile.hpp"
#include "request.hpp"
#include "respons.hpp"
#include <fcntl.h>
# include <sys/time.h>
#include <unistd.h>
#include <signal.h>
# include <sys/stat.h>

class   cl;
std::string tostring(size_t n);

class cgi
{
private:
    bool                                first;
    std::map<std::string, std::string>  env_m;
    int                                 in_fd;
    char                                **env;
    char                                **args;
    pid_t                               pid;
    int                                 status;
    int                                 out_fd;
    struct timeval                      begin;
public:
    std::string                         output;
    std::ifstream    in;
    cgi();
    cgi(const cgi& other);
    cgi&    operator=(const cgi& other);
    ~cgi();
    void        cgi_init(const one_server& server, const request& req, const std::string& input);
    void        set_cgi(const Location& location, const std::string& path);
    void        cgi_execute();
    void        cgi_out(one_server& server, cl& client, const std::string& input);
    std::string cgi_run(one_server& server, cl& client, const std::string& input);
};

std::string    create_file_name();

#endif