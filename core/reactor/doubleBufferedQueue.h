#ifndef _DOUBLE_BUFFERED_QUEUE_H_INCLUDED_
#define _DOUBLE_BUFFERED_QUEUE_H_INCLUDED_
#include <queue>
#include <mutex>
using std::queue;
using std::mutex;
//#define DEFAULT_MAX_EVS 10000


//拥有读写锁的双缓冲队列

template <class T>
class doubleBufferedQueue
{
private:
    queue<T> queue1;
    queue<T> queue2;
    queue<T>* reader;
    queue<T>* writer;
    /*
    这里设计了读锁和写锁，是为了避免在交换读写队列时
    线程读写队列造成错误。其他时间读锁和写锁不会被其他线程占用，
    因此不会影响读写效率。
    */
    mutex w_mutex;
    mutex r_mutex;
public:
    doubleBufferedQueue();
    ~doubleBufferedQueue();
    // writing thread
    void Push(T& val);
    // reading thread
    T& Read();
    // get total size
    size_t Size();

};



#endif