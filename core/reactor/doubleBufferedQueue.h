#ifndef _DOUBLE_BUFFERED_QUEUE_H_INCLUDED_
#define _DOUBLE_BUFFERED_QUEUE_H_INCLUDED_
#include <queue>
#include <mutex>
#include <vector>
using std::queue;
using std::mutex;
using std::vector;

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
    // writing thread
    void Push(T& val);
    void Push(vector<T>& val_array, int size); //支持批量写入队列以减少锁的使用
    // reading thread
    T& Read();
    // get total size
    size_t Size();
    bool Empty();

};

template <class T>
doubleBufferedQueue<T>::doubleBufferedQueue():
queue1(),queue2(),reader(nullptr),writer(nullptr),w_mutex(),r_mutex()
{
    reader = &queue1;
    writer = &queue2;
}

template<class T>
void doubleBufferedQueue<T>::Push(T& val)
{
    w_mutex.lock();
    writer->push(val);
    w_mutex.unlock();
}

template<class T>
void doubleBufferedQueue<T>::Push(vector<T>& val_array, int size)
{
    w_mutex.lock();

    for(int i = 0; i < size; i++){
        T& value = val_array[i];
        writer->push(value);
    }

    w_mutex.unlock();
}

//队列为空时，Read会返回一个T类型变量的引用，该变量的地址空间全部被置零
//建议在访问队列前先调用Empty()判断队列是否为空
template<class T>
T& doubleBufferedQueue<T>::Read()
{
    r_mutex.lock();//除非交换读写队列，否则这个读锁不会被其他线程占用
    //读队列为空时交换读写队列
    if(reader->empty()){ 
        if(!writer->empty()){ 
            //交换队列时需要锁住读写队列
            //这里已经锁住读队列了，因此只需要获取写队列的锁
            w_mutex.lock();
            //交换读写队列其实就是交换reader和writer指针
            queue<T>* t = reader;
            reader = writer;
            writer = t;
            w_mutex.unlock();
        }
    }
    T& temp = reader->front();
    if(!reader->empty())
        reader->pop();
    r_mutex.unlock();
    return temp;
}


template<class T>
size_t doubleBufferedQueue<T>::Size()
{
    return queue1.size()+queue2.size();
}

template<class T>
bool doubleBufferedQueue<T>::Empty()
{
    return (queue1.empty() && queue2.empty());
}


#endif