#ifndef _SHM_MUTEX_H_INCLUDED_
#define _SHM_MUTEX_H_INCLUDED_
#define _NONBLOCKING_
#include "shmem.h"
#include <semaphore.h>


class shmMutex : public sharedMemory
{
private:
    void* buffStart = NULL;
    void* start = NULL;
    sem_t* mutex = NULL;
    bool initMutex(); //初始化互斥信号量

public:
    shmMutex();
    _NONBLOCKING_ bool tryMutex(); //获取互斥锁（非阻塞）
    void* shmAccess(); //访问共享内存区域（非阻塞）
    _NONBLOCKING_ bool rlsMutex (); //释放互斥锁（非阻塞）
    bool destroyMutex(); //删除互斥锁
    ~shmMutex();
};


#endif