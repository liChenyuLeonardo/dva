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
        void* end; //共享内存的终止地址（最后一个地址的下一个）
        int shmId; //共享内存的id
        bool isAttached;

        
    public:
        //指定大小的构造函数，该构造函数会创建一个全新的私有共享内存段，只用于父子进程间的通信
        sharedMemory(size_t size);
        //指定共享内存id的构造函数，会调用attachShm()初始化共享内存的大小，起始地址等信息
        sharedMemory(int shmid, size_t size);
        //指定key的共享内存段，该构造函数也会创建一个新的共享内存段，可以用于任意进程间的通信
        sharedMemory(size_t size, key_t theKey);
        void* getStartAddr();
        void* getEndAddr();
        size_t getSize();
        int getshmId();
        //释放共享内存段，注意该函数只能由创建该共享内存段的进程或者超级用户进程调用
        bool remove();
        //连接共享内存段
        void* attach();
        //断开连接共享内存段，该函数不会导致共享内存段被释放
        bool detach();
};

#endif