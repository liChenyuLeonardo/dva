#include "processManager.hpp"
// to be improved

processManager::processManager(int processes):procList()
{
         this->processes = processes;
    if(processes > MAX_PROCESSES){
       isRunnable = false;
       // logger::log(ERROR, "processManager", "exceeded the specified max num of processes");
    }
    else{
        isRunnable = true;
    }
}

processManager::processManager():procList()
{
    isRunnable = true;
    processes = 4;
}

bool processManager::launcher(string path, string config)
{
    if(!isRunnable){
         cout<< "non-runnable!" <<endl;
        return false;
    }
    for(int i = 0; i < processes; i++){
        pid_t cpid = launchNew(path, config);
        if(cpid == -1){
            this->stopAll();
            cout<< "launch failed!" <<endl;
            // logger::log(FATAL_ERROR, "processManager::launcher", "launch failed!");
            return false;
        }
    }
    return true;
}

pid_t processManager::launchNew (string path, string config)
{
        if(!isRunnable){
        return -1;
    }
    if(procList.getCurrentNum()+1 > this->processes){
       // cout<< "exceeded the specified max num of processes" <<endl;
       // logger::log(ERROR, "processManager::launchNew", "exceeded the specified max num of processes");
        return -1;
    }
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
                     return -1;
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
    if(kill(pid, signo) != 0){
        //logger::log(SYS_ERROR, "processManager::sendSig", "sendSig error")
        return false;
    }
    else
        return true;
}

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
}

bool processManager::stopSingl(pid_t pid)
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

void processManager::stopAll() // terminate the process gently
{
        process_t* temp = procList.getHead();
    if(temp == NULL){
        // logger::log(ERROR, "processManager::sendSigAll", " no process in the procList!");
        return;
    }
    while(temp != NULL){
        stopSingl(temp->pid);
        temp = temp->next;
    }
}

bool processManager::shutdownSingl(pid_t pid)
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

void processManager::shutdownAll()
{
    process_t* temp = procList.getHead();
    if(temp == NULL){
        // logger::log(ERROR, "processManager::sendSigAll", " no process in the procList!");
        return;
    }
    while(temp != NULL){
        shutdownSingl(temp->pid);
        temp = temp->next;
    }
}

bool processManager::reloadAll(string path, string config)
{
     if(path.empty()){
        // logger::log(ERROR, "processManager::reloadAll", " no execution path is specified!");
        return false;
    }
    stopAll();
    return launcher(path, config);
}