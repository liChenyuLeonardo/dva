#include "epollEngine.h"

epollEngine::epollEngine()
{
    this->eventList.resize(NUM_OF_EVENTS); //以NUM_OF_EVENTS作为单位对evenList进行扩容
    epollInit(); // 初始化epoll
}


bool epollEngine::epollInit()
{
    int fd = epoll_create1(0);
    if(fd == -1){
        //epoll create failed
        return false;
    }
    this->epollfd = fd;
    return true;
}

void epollEngine::setEpollTimeout(int timeout)
{
    this->timeout = timeout;
}

bool epollEngine::epollAddEvent(epoll_event ev)
{
    if(this->epollfd < 0){
        //invalid epollfd
        return false;
    }
    if( epoll_ctl(this->epollfd, EPOLL_CTL_ADD, ev.data.fd, &ev) == 0 ){
        ++nfds;
        return true;
    }else{
         //add-event failed
         return false;
    }
}

bool epollEngine::epollDelEvent(epoll_event ev)
{
    if(this->epollfd < 0){
        //invalid epollfd
        return false;
    }
    if( epoll_ctl(this->epollfd, EPOLL_CTL_DEL, ev.data.fd, &ev) == 0 ){
        --nfds;
        return true;
    }else{
         //delete-event failed
         return false;
    }
}

epoll_event epollEngine::setEpollEvent(int fd, uint32_t op)
{
    struct epoll_event  ev;
    ev.events = op;
    ev.data.fd = fd;
    return ev;
}

int epollEngine::epollWait()
{
    int ret = epoll_wait(epollfd, &*eventList.begin(),(int)eventList.size(), this->timeout);
    this->nfds = ret;
        if(ret < 0){
            //epoll_wait failed
            return ret;
        }else{
            if( ret == (int)eventList.size() ){
                int currentSize = eventList.size();
                eventList.resize(currentSize + NUM_OF_EVENTS); //如果接受的事件数量达到了eventList的容量，就对eventList进行线性扩容
            }
            return ret;
        }
}

int epollEngine::getnfds()
{
    return this->nfds;
}

vector<epoll_event>& epollEngine::getEventList()
{
    return this->eventList;
}

epollEngine::~epollEngine(){
    
}