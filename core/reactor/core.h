#ifndef _CORE_H_INCLUDED_
#define _CORE_H_INCLUDED_
#include "lib/Socket.h"
#include "doubleBufferedQueue.h"
#include "epollEngine.h"
#include "module/module.h"
#include "threadPool.h"

struct event_t
{
    int fd;
    int type;
    event_t() = delete;
    event_t(int fd, int type):fd(fd),type(type){}
    event_t(const epoll_event& ep)
    {
        fd = ep.data.fd;
        switch (ep.events)
        {
        case (EPOLLIN | EPOLLET):
            type = 1;
            break;
        case (EPOLLET | EPOLLOUT):
            type = 2;
            break;
        default: //先只简单处理边缘触发的输入和输出事件，其他事件日后再说
            type = 0;
            break;
        }
    }
    event_t& operator= (const epoll_event& ep)
    {
        fd = ep.data.fd;
        switch (ep.events)
        {
        case (EPOLLIN | EPOLLET):
            type = 1;
            break;
        case (EPOLLET | EPOLLOUT):
            type = 2;
            break;
        default: //先只简单处理边缘触发的输入和输出事件，其他事件日后再说
            type = 0;
            break;
        }
    }
};

#endif