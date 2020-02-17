#include "../core/reactor/Acceptor.h"
#include <unistd.h>
#include <iostream>
using namespace std;

int main()
{
    epollEngine epoll_instance;
    doubleBufferedQueue<epoll_event> queue_instance;
    int serverSocket = Socket::initListenSocket("127.0.0.1", 1234, 1000, 1);
    if(serverSocket < 0){
        perror("socket init error");
        return 0;
    }
    cout << "The socket is " << serverSocket <<endl;
    Acceptor acceptor_instance(epoll_instance, queue_instance, serverSocket);
    acceptor_instance.run(); // off you go
    char a;
    while(true){
        if(queue_instance.Empty()){
            cout << "queue is fucking empty and size = "<< queue_instance.Size() << endl; 
            sleep(2);
            continue;
        }
        epoll_event temp;
        temp = queue_instance.Read();
        cout << "new socket received! fd is " << temp.data.fd <<  endl;
	char buffer[1024];
	if(temp.data.fd != serverSocket){
	   Socket haha(temp.data.fd, CLIENT_SOCKET);
	   if(haha.Recv((void*)buffer, 1024) > 0)
	   	cout << buffer << endl;
	//close(temp.data.fd);
	}
    }
    return 0;
}
