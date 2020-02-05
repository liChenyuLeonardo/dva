#include "shmem.hpp"


sharedMemory::sharedMemory(size_t size):
start(NULL),shmId(-1),isAttached(false),_size(0)
{
    if(size <= 0){
        //无效的共享内存空间大小
    }
    else{
        _size = size;
        shmId = shmget(IPC_PRIVATE, _size, IPC_CREAT | IPC_EXCL | USER_ALLOWANCE);
        if(shmId < 0){
            //开辟共享内存空间失败
        }
        else{
            start = attach();
            memset(start, 0, _size);
        }
    }
}

sharedMemory::sharedMemory(int shmid, size_t size):
start(NULL),shmId(-1),isAttached(false),_size(0)
{
    this->shmId = shmid;
    this->_size = size;
    start = attach();
}
/*
sharedMemory::sharedMemory(size_t size, key_t theKey):
start(NULL),end(NULL),shmId(-1),isAttached(false),_size(0)
{
    if(size <= 0){
        //无效的共享内存空间大小
    }
    else{
        _size = size;
        shmId = shmget(theKey, _size, IPC_CREAT | IPC_EXCL | USER_ALLOWANCE);
        if(shmId < 0){
            //开辟共享内存空间失败
        }
        else{
            start = attach();
            memset(start, 0, _size);
            end = start + _size + 1;
        }
    }
}*/

void* sharedMemory::attach()
{
    void* a = shmat(shmId, NULL, 0);
    if(a != (void*) -1){
        isAttached = true;
        return a;
    }else
    {
        isAttached = false;
        return NULL;
    }
    
}

void* sharedMemory::getStartAddr()
{
    return this->start;
}

int sharedMemory::getshmId()
{
    return this->shmId;
}

size_t sharedMemory::getSize()
{
    return this->_size;
}

bool sharedMemory::isAttach()
{
    return this->isAttached;
}

bool sharedMemory::remove()
{
    return  shmctl(shmId, IPC_RMID, NULL) == -1 ? false : true;
}

bool sharedMemory::detach()
{
    return  shmdt(shmId) == -1 ? false : true;
}

sharedMemory::~sharedMemory()
{
    if(!detach()){
        //共享内存脱离连接失败
    }
    else
    {
        //进程（pid为getpid()）已脱离共享内存（shmId为 this->shmId）的连接
    }
    
}