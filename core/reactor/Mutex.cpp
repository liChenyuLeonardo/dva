#include "Mutex.h"

Mutex::Mutex():mutex(nullptr)
{
    if(!initMutex()){
        if(mutex != nullptr){
            delete mutex;
        }
        mutex = nullptr;
    }
}

bool Mutex::initMutex()
{
    mutex = new sem_t;
    if(sem_init(mutex, 1, 1) == -1) return false;
    return true;
}

bool Mutex::waitMutex()
{
    if(mutex == nullptr) return false;
    return sem_wait(mutex) == -1 ? false : true;
}

bool Mutex::tryMutex()
{
    if(mutex == nullptr) return false;
    return sem_trywait(mutex) == -1 ? false : true;
}

bool Mutex::rlsMutex()
{
    if(mutex == nullptr) return false;
    return sem_post(mutex) == -1 ? false : true;
}


bool Mutex::destroyMutex()
{
    if(mutex == nullptr) return false;
    return sem_destroy(mutex) == -1 ? false : true;
}

bool Mutex::isMutexAvailable()
{
    return mutex == nullptr ? false : true;
}

Mutex::~Mutex()
{
    if(mutex != nullptr){
        while(tryMutex() == false); //lock is occupied, wait for the lock to be released
        destroyMutex();
        delete mutex;
    }
}