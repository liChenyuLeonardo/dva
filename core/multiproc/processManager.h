#ifndef _PROCESS_MANAGER_H_
#define _PROCESS_MANAGER_H_

#include <signal.h>
#include <unistd.h>
#include "lib/process.h"

#define SIG_GENTLE_STOP  SIGINT
#define SIG_INSTANT_STOP SIGTERM

class processManager
{
private:
    processList procList;
    //void setErrMsg(const char* msg);
    void sendSigAll(int signo);
    bool sendSigSingl(pid_t pid, int signo); 
public:
    processManager();
    //启动一个新进程
    pid_t launch(string path, string config);  
    // 粗暴地终止单个进程
    bool shutdown(pid_t pid); 
    //优雅地终止单个进程
    bool stop(pid_t pid); 
    //重新启动所管理的进程。
    bool reload(pid_t pid, string path, string config); 
   // bool changeStatus(pid_t pid, pstatus_t pstat); //通知manager更改进程状态，用于子进程异常退出的时候，由父进程调用来更改进程管理器状态
    //~processManager();
};
#endif