#include "shmMutex.hpp"

shmMutex::shmMutex():
sharedMemory(PAGE_SIZE),mutex(NULL),buffStart(NULL),start(NULL)
{
    if(getStartAddr() != NULL && isAttach()){
        this->start = getStartAddr();
        if(initMutex() == false){
            buffStart = NULL;
            mutex = NULL;
            start = NULL;
        }
    }
}

bool shmMutex::initMutex()
{
    if(start == NULL) return false;
    mutex = (sem_t*) start;
    if(sem_init(mutex, 1, 1) == -1) return false;
    /*共享内存的内存模型
    |mutex(sem_t*)|
    |-------------|
    | user space  |--->buffStart
    | ...         |
    */
    buffStart = (void*) (mutex + 1); 
}

_NONBLOCKING_ bool shmMutex::tryMutex()
{
    return sem_trywait(mutex) == -1 ? false : true;
}

_NONBLOCKING_ bool shmMutex::rlsMutex()
{
    return sem_post(mutex) == -1 ? false : true;
}

void* shmMutex::shmAccess()
{
    if(tryMutex()){
        return buffStart;
    }else{
        return NULL;
    }
}

bool shmMutex::destroyMutex()
{
    return sem_destroy(mutex) == -1 ? false : true;
}

shmMutex::~shmMutex()
{
    if(destroyMutex() == false){
        //共享内存互斥锁解除失败，可能已经被解除
    }

}