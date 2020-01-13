#include "shmem.hpp"

sharedMemory::sharedMemory(size_t size)
{
    if(size == 0){
        _size = 0;
        start = NULL;
        end = NULL;
        shmid = -1;
    }
    else{
        _size = size;
        shmid = shmget(IPC_PRIVATE, _size, IPC_CREAT | IPC_EXCL | USER_ALLOWANCE);
        if(shmid < 0){
            //logger::log(SYS_ERROR,"sharedMemory::sharedMemory", "开辟共享内存空间失败")
        }
        else{
            start = attachShm();
        }
    }
}