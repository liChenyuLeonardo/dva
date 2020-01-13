#ifndef _PROCESS_MANAGER_HPP_
#define _PROCESS_MANAGER_HPP_
#include <signal.h>
#include<unistd.h>
#include"../lib/process.hpp"
#define SIG_GENTLE_STOP  SIGINT
#define SIG_INSTANT_STOP SIGTERM
//调试用
#include <iostream>
using std::cout;
using std::endl;

class processManager
{
private:
    processList procList;
    int processes;
    int isRunnable;
    //void setErrMsg(const char* msg);
    void sendSigAll(int signo);
    bool sendSigSingl(pid_t pid, int signo); 
public:
    processManager(int processes);
    processManager();
    //启动所有进程
    bool launcher(string path, string config); 
    //启动一个新进程
    pid_t launchNew(string path, string config); 
    //粗暴地终止所有进程
    void shutdownAll(); 
    // 粗暴地终止单个进程
    bool shutdownSingl(pid_t pid); 
    //优雅地终止所有进程
    void stopAll(); 
    //优雅地终止单个进程
    bool stopSingl(pid_t pid); 
    //重新启动所管理的进程。
    bool reloadAll(string path, string config); 
   // bool changeStatus(pid_t pid, pstatus_t pstat); //通知manager更改进程状态，用于子进程异常退出的时候，由父进程调用来更改进程管理器状态
    //~processManager();
};
#endif