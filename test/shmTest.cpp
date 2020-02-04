#include <iostream>
#include <string.h>
#include "../lib/shmem.hpp"
#include <unistd.h>
#include <sys/wait.h>
using namespace std;
struct msg{
    bool isWritten = false;
    char message[100];
};
int main()
{
    sharedMemory shm(1024);//a sharedMem with the size of 1024 bytes
    int shmId = shm.getshmId();
    msg* start = (msg*)shm.getStartAddr();
    if(start != NULL)
        cout<< "shm attached at "<< start << endl;
    else{
        perror("shm create error");
        exit(0); 
    }
    memset((void*)start, 0, sizeof(msg));
    int pid = fork();
    if(pid == 0){
        start->isWritten = true;
        strcpy(start->message,"Hello World!\n");
        shm.detach();
        exit(0);
    }
    else{
        int status;
        wait(&status);
        if(start->isWritten == true){
            cout<<start->message<<endl;
        }
        else{
            cout<<"error";
        }
        shm.detach();
    }
    

}