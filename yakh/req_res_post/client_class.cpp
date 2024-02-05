#include "client_class.hpp"
#include "request.hpp"
#include "respons.hpp"
#include "post.hpp"

cl::cl():buffer(buffer_size, 0)  ,cgi(), post(), req(),res() 
{
    end_send = 0;
    test1 = 0;
    test2 = 0;
    test3 = 0;
    lock = 0;
}

cl::cl(const cl& other)//: post(), req()
{
    *this = other;
}

cl& cl::operator=(const cl& other)
{
    this->post = other.post;
    this->test1 = other.test1;
    this->test2 = other.test2;
    this->test3 = other.test3;
    this->lock = other.lock;
    return (*this);
}

cl::~cl(){}