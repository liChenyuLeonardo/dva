#ifndef _MUTEX_H_INCLUDED_
#define _MUTEX_H_INCLUDED_
#define _NONBLOCKING_
#include <semaphore.h>

class Mutex
{
private:
    sem_t* mutex;
    bool initMutex(); //初始化互斥信号量

public:
    Mutex();
    bool tryMutex(); //获取互斥锁（非阻塞）
    bool waitMutex(); //
    bool rlsMutex (); //释放互斥锁（非阻塞）
    bool destroyMutex(); //删除互斥锁
    bool isMutexAvailable(); //test if mutex is available
    ~Mutex();
};

#endif