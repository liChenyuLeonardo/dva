#ifndef EPOLL_H_INCLUDED
#define EPOLL_H_INCLUDED
#include <sys/epoll.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <vector>

using std::vector;
#define NUM_OF_EVENTS 500 //设定的epoll事件数为500
#define DEFAULT_TIMEOUT 300 //epoll_wait的等待时间

class epollEngine
{
private:
    int epollfd = -1; 
    vector<epoll_event> eventList; //epoll事件容器
    int nfds = 0; //当前epoll侦听的fd数量
    int timeout = DEFAULT_TIMEOUT; //epoll_wait的等待时间 
    void epollInit();

public:
    epollEngine();
    bool epollAddEvent(epoll_event ev);
    bool epollDelEvent(epoll_event ev);
    int epollWait();
    epoll_event setEpollEvent(int fd, uint32_t op);
    void setEpollTimeout(int timeout); //设置自定义的epoll延时
    vector<epoll_event>& getEventList();
    int getnfds();
    ~epollEngine();
};
#endif