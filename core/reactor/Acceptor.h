#ifndef _ACCEPTOR_H_INCLUDED_
#define _ACCEPTOR_H_INCLUDED_
#include "epollEngine.h"
#include "doubleBufferedQueue.h"
#include "lib/Socket.h"
#include <thread>
#include <string>
#include "lib/type.h"
using std::thread;
using std::string;

class Acceptor
{
private:
    epollEngine& IOMultiplexer;
    doubleBufferedQueue<epoll_event>& IOQueue;
    int serverSocket;
    int status;
    string errMsg;
    //禁用拷贝构造函数
    Acceptor(const Acceptor&) = delete;
    Acceptor& operator= (const Acceptor&) = delete;

public:
    Acceptor(epollEngine&, doubleBufferedQueue<epoll_event>&, int);
    void AcceptorCycle();
    bool run();
    ~Acceptor();
};


#endif