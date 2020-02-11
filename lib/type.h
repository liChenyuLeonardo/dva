#ifndef _TYPE_H_INCLUDED_
#define _TYPE_H_INCLUDED_
#include <string>
using std::string;

struct event_t{
    enum evType{
        SOCKET_IN = 1,
        SOCKET_OUT = (1 << 1),
        DISK_IN = (1 << 2),
        DISK_OUT = (1 << 3),
        UNAVAILABLE = (1 << 4)
    };
    int fd;
    pid_t pid;
    evType eventType;

    event_t(int fd, pid_t pid, evType type):
    fd(fd),pid(pid),eventType(type){
    }

    event_t():
    fd(-1),pid(-1),eventType(UNAVAILABLE){
    }   
};

const int DVA_OK = 1;
const int DVA_ERROR = -1;
const int DVA_FAILED = 0;

#endif