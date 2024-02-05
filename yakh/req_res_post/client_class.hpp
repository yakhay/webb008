#ifndef CLIENT_CLASS_HPP
#define CLIENT_CLASS_HPP

#include "post.hpp"
#include "request.hpp"

class   cl
{
public:
    cl();
    static const size_t buffer_size = 1024;
    std::vector<char> buffer;
    std::ifstream   fileStream;
    cgi             cgi;
    post            post;
    request         req;
    respons         res;
    int             test1;
    int             test2;
    int             test3;
    bool            end_send;
    int             lock;
    cl(const cl& other);
    cl& operator=(const cl& other);
    ~cl();
};

#endif