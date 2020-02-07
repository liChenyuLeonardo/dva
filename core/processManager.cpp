#include "processManager.hpp"
// to be improved

processManager::processManager():procList()
{
}


pid_t processManager::launchNew (string path, string config)
{
    if(path.empty()){
        //cout<<" no execution path is specified!" <<endl;
        // logger::log(ERROR, "processManager::launchNew", " no execution path is specified!");
        return -1;
    }
    pid_t pid = fork();
    if(pid < 0){
        //cout << "fork error"<<endl;
        //logger::log(SYS_ERROR, "processManager::launchNew", "fork error")
        return -1;
    }
    else if(pid == 0){
        //cout<<"child process, pid = " << getpid()<<endl;
        if(execl(path.c_str(), config.c_str()) < 0){
            //to be done: 这里应该用管道告知父进程子进程出现错误
            //logger::log(SYS_ERROR, "processManager::launchNew", "exec error")
            exit(-1);
        }
    }
    else{
        process_t* newProcess = new process_t(pid, config, path);
        procList.insertList(newProcess);
        return pid;
    }
}

bool processManager::sendSigSingl(pid_t pid, int signo)
{
    if(procList.searchList(pid) == NULL){
        //the specified process cannot be found in the process list
        return false;
    }
    if(kill(pid, signo) != 0){
        //logger::log(SYS_ERROR, "processManager::sendSig", "sendSig error")
        return false;
    }
    else
        return true;
}
/*
void processManager::sendSigAll(int signo)
{
    process_t* temp = procList.getHead();
    if(temp == NULL){
        // logger::log(ERROR, "processManager::sendSigAll", " no process in the procList!");
        return;
    }
    while(temp != NULL){
        sendSigSingl(temp->pid, signo);
        temp = temp->next;
    }
}*/

bool processManager::stop(pid_t pid)
{
    //以后改成sigsuspend
    if(sendSigSingl(pid, SIG_GENTLE_STOP)){
        pause();
        procList.removeList(pid);
        return true;
    }else{
        return false;
    }
}

bool processManager::shutdown(pid_t pid)
{
    //以后改成sigsuspend
    if(sendSigSingl(pid, SIG_INSTANT_STOP)){
        cout << "pause to wait for child to stop: )"<<endl;
       pause();
       procList.removeList(pid);
        return true;
    }else{
        return false;
    }
}


bool processManager::reload(pid_t pid, string path, string config)
{
     if(path.empty()){
        // logger::log(ERROR, "processManager::reloadAll", " no execution path is specified!");
        return false;
    }
    stop(pid);
    return launch(path, config);
}