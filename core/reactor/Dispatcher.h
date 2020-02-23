#ifndef _DISPATCHER_H_INCLUDED_
#define _DISPATCHER_H_INCLUDED_
#include "core.h"
const int DEFAULT_NUM_OF_THREADS = 4; //to do：设置线程数为cpu数目


struct dispatcher_data_t
{
    doubleBufferedQueue<epoll_event>& IOQueue;
    epollEngine& IOMultiplexer;
    module& modules;
    dispatcher_data_t(doubleBufferedQueue<epoll_event>& a, epollEngine& b, module& c): 
    IOQueue(a),IOMultiplexer(b),modules(c){}
    dispatcher_data_t(const dispatcher_data_t& a) = default;
    //to be done
};



class Dispatcher
{
private:
    doubleBufferedQueue<epoll_event>& IOQueue;
    threadPool workerPool;
    epollEngine& IOMultiplexer;
    module& modules;
public:
    inline Dispatcher(dispatcher_data_t& init_data):
    IOQueue(init_data.IOQueue), 
    workerPool(DEFAULT_NUM_OF_THREADS),
    IOMultiplexer(init_data.IOMultiplexer),
    modules(init_data.modules){};

    void reader(event_t& event);
    void writer(event_t& event);
    void dispatcher_cycle();
    void run(); // move your ass!
    ~Dispatcher();
};


#endif