#include "../core/reactor/Acceptor.h"
#include <unistd.h>
#include <iostream>
using namespace std;

int main()
{
    epollEngine epoll_instance;
    doubleBufferedQueue<epoll_event> queue_instance;
    int serverSocket = Socket::initListenSocket("127.0.0.1", 1234, 1000);
    if(serverSocket == -1){
        perror("socket init error");
        return 0;
    }
    Acceptor acceptor_instance(epoll_instance, queue_instance, serverSocket);
    acceptor_instance.run(); // off you go
    while(true){
        if(queue_instance.Empty()){
            cout << "queue is fucking empty" << endl; 
            usleep(200);
            continue;
        }
        epoll_event temp;
        temp = queue_instance.Read();
        cout << "new socket received! fd is " << temp.data.fd << endl;
    }
    return 0;
}