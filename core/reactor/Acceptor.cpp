#include "Acceptor.h"

/*---调试用 ---*/
#include<iostream>
using std::cout;
using std::endl;

Acceptor::Acceptor(epollEngine& ep, doubleBufferedQueue<epoll_event>& q, int s):
IOMultiplexer(ep), IOQueue(q), serverSocket(s), status(0)
{

}

void Acceptor::AcceptorCycle()
{
    int size = 0;
    Socket conn_sock;
    int accept_sock = -1;
    epoll_event temp;
    if(this->serverSocket == -1){
        //log: invalid serverSocket
        return;
    }
    temp = this->IOMultiplexer.setEpollEvent(this->serverSocket, EPOLLIN | EPOLLET);
    this->IOMultiplexer.epollAddEvent(temp); //将监听socket添加入epoll中，监听socket设为ET
    //main circle
    while(true){
        if((size=this->IOMultiplexer.epollWait()) == 0){
	    cout << "empty set" << endl;
	    goto check;
	}
        else if(size < 0){
            status = -1;
            cout << "epollWait failed!"<<endl;
            break;
        }
        else{
            vector<epoll_event>& events = this->IOMultiplexer.getEventList();
            this->IOQueue.Push(events, size);
            for(auto& val : events){
                if(val.data.fd == this->serverSocket){
                    conn_sock.changeSocket(this->serverSocket, SERVER_SOCKET);
                    while((accept_sock=conn_sock.Accept()) > 0){
                        temp = this->IOMultiplexer.setEpollEvent(accept_sock, EPOLLIN | EPOLLET);
                        this->IOMultiplexer.epollAddEvent(temp);
                    }

                    if (conn_sock.errno != EAGAIN && conn_sock.errno != ECONNABORTED && conn_sock.errno != EPROTO && conn_sock.errno != EINTR){
                        this->status = -1;
                        cout<< "accept error" << endl;
                    }
                    break;
                }
            }
        }
    check:
        if(this->status < 0) break;
    }
    cout<< "Acceptro main thread exited!"<<endl;
}

void Acceptor::run()
{
    status = 0;
    std::thread AcceptorThread(&Acceptor::AcceptorCycle, this);
    AcceptorThread.detach();
}
