#include "doubleBufferedQueue.h"

template <class T>
doubleBufferedQueue<T>::doubleBufferedQueue():
queue1(queue<T>()),queue2(queue<T>()),reader(nullptr),writer(nullptr),w_mutex(mutex())
{
    reader = &queue1;
    writer = &queue2;
}

template<class T>
doubleBufferedQueue<T>::~doubleBufferedQueue(){

}

template<class T>
void doubleBufferedQueue<T>::Push(T& val)
{
    w_mutex.lock();
    writer->push(val);
    w_mutex.unlock();
}

template<class T>
T& doubleBufferedQueue<T>::Read()
{
    T& temp;
    r_mutex.lock();//除非交换读写队列，否则这个读锁不会被其他线程占用
    temp = reader->front();
    reader->pop(); //从队列取出元素
    //读队列为空时交换读写队列
    if(reader->empty()){  
        //交换队列时需要锁住读写队列
        //这里已经锁住读队列了，因此只需要获取写队列的锁
        w_mutex.lock();
        //交换读写队列其实就是交换reader和writer指针
        queue<T>* t = reader;
        reader = writer;
        writer = t;
        w_mutex.unlock();
    }
    r_mutex.unlock();

    return temp;
}


template<class T>
size_t doubleBufferedQueue<T>::Size()
{
    return reader->size() + writer->size();
}

