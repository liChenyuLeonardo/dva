#ifndef _ACCEPTOR_H_INCLUDED_
#define _ACCEPTOR_H_INCLUDED_
#include "epollEngine.h"
#include "doubleBufferedQueue.h"
#include "../lib/Socket.h"
#include <thread>
#include "../lib/type.h"

class Acceptor
{
private:
    epollEngine& IOMultiplexer;
    doubleBufferedQueue<epoll_event>& IOQueue;
    int serverSocket;
    int status;
    //禁用拷贝构造函数
    Acceptor(const Acceptor&) = delete;
    Acceptor& operator= (const Acceptor&) = delete;

public:
    Acceptor(epollEngine& ep, doubleBufferedQueue<epoll_event>& q, int s);
    void AcceptorCycle();
    void run();
    ~Acceptor() = default;
};


#endif