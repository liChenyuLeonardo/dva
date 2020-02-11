#ifndef _PROCESS_H_INCLUDED_
#define _PROCESS_H_INCLUDED_
#include <unistd.h>
#include <string>
#include<iostream>
using std::string;
using std::cout;
using std::endl;
#define MAX_PROCESSES 1024

#define list_for_each(head) while(head != NULL)
#define chlist(_next, _former, a) a->next = _next,  a->former = _former;
/*enum pstatus_t{
    UNSET,RUNNING, STOPING, SHUTING
};*/

// process descriptor
 struct process_t {
     //pstatus_t pstatus;
     int exitStatus;
     pid_t pid;
     string conf;
     string path;
     process_t* next;
     process_t* former;

     process_t(/*pstatus_t pstatus, */pid_t pid, string conf, string path)
     {
     //    this->pstatus = pstatus;
         this->pid = pid;
         this->conf = conf;
         this->path = path;
         this->next = NULL;
         this->former =NULL;
     }

     process_t()
     {
       //  pstatus = UNSET;
         pid = -1;
         exitStatus = 0;
         next= NULL;
         former = NULL;
     }
 };
 
//进程队列，用于进程管理器管理进程
class processList{
    private:
        int currentNum;
        process_t* head;
        process_t* rear;
    public:
        processList();
        bool insertList(process_t* theProcess);
        bool removeList(pid_t pid);
        process_t* searchList(pid_t pid);
     //   bool changeStatus(pid_t pid , pstatus_t status);
       process_t*  getHead();
        bool changeConf(pid_t pid, string& conf);
        bool changePath(pid_t pid, string& path);
        int getCurrentNum();
        ~processList();

};
#endif