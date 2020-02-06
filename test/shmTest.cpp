#include <iostream>
#include <string.h>
#include "../lib/shmMutex.hpp"
#include <unistd.h>
#include <sys/wait.h>
using namespace std;
struct msg{
    bool isWritten = false;
    char message[100];
};
int main()
{
    shmMutex haha;
    fork();
    fork();
    void* start = NULL;
    
    while(start == NULL){
        start = haha.shmAccess();
        usleep(700);
        cout<< "pid " << getpid() << " is requesting for mutex lock" << endl;
    }
    cout<<"pid "<< getpid()<< " has acquired the mutex lock" << endl;
    if(haha.rlsMutex() == false){
        cout<<"pid "<< getpid()<< " failed in releasing the mutex lock" << endl;
    }
    else{
        cout<<"pid "<< getpid()<< " has released the mutex lock" << endl;
    }
    exit(0);    

}