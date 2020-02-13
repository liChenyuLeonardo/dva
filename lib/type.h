#ifndef _TYPE_H_INCLUDED_
#define _TYPE_H_INCLUDED_
#include <string>
using std::string;

struct event_t{
    enum evType{
        IN = EPOLL_IN,
        OUT = EPOLL_OUT,
        
        ERROR = (1 << 2),
        HUP = (1 << 3),
        UNAVAILABLE = (1 << 4)
    };
    int fd;
    evType eventType;

    event_t(int fd, evType type):
    fd(fd),eventType(type){
    }

    event_t():
    fd(-1),eventType(UNAVAILABLE){
    }   
};

const int DVA_OK = 1;
const int DVA_ERROR = -1;
const int DVA_FAILED = 0;

#endif