#ifndef _SHM_MUTEX_HPP_
#define _SHM_MUTEX_HPP_
#define _NONBLOCKING_
#include "shmem.hpp"

class shmMutex : public sharedMemory
{
private:
    void* buffStart = NULL;
    void* start = NULL;
    sem_t* mutex = NULL;
    bool initMutex(); //初始化互斥信号量
    bool destroyMutex(); //释放互斥锁

public:
    shmMutex();
    _NONBLOCKING_ bool tryMutex(); //获取互斥锁（非阻塞）
    void* shmAccess(); //访问共享内存区域（非阻塞）
    _NONBLOCKING_ bool rlsMutex (); //释放互斥锁（非阻塞）
    ~shmMutex();
};


#endif