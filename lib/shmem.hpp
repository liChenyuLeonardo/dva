#ifndef SHMEM_HPP_DEFINED
#define SHMEM_HPP_DEFINED
#include <unistd.h>
#include <sys/shm.h>

#define USER_ALLOWANCE 0660//共享内存段的访问权限，默认是拥有者和同组用户可读可写
#define PAGE_SIZE getpagesize()
//用于开辟一块共享内存区域
class sharedMemory
{
    private:
        size_t _size; //共享内存段的大小
        void* start; //共享内存的起始地址
        void* end; //共享内存的终止地址
        int shmid; //共享内存的id

        
    public:
        //指定大小的构造函数，该构造函数会创建一个全新的共享内存段
        sharedMemory(size_t size);
        //指定共享内存id的构造函数，会调用attachShm()初始化共享内存的大小，起始地址等信息
        sharedMemory(int shmid, size_t size);
        void* getStartAddr();
        void* getEndAddr();
        void* getSize();
        int getshmid();
        //释放共享内存段，注意该函数只能由创建该共享内存段的进程或者超级用户进程调用
        bool rmShm();
        //连接共享内存段
        void* attachShm();
        //断开连接共享内存段，该函数不会导致共享内存段被释放
        bool detachShm();
}


#endif