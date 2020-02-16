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
    while(this->status >= 0){
        if((size=this->IOMultiplexer.epollWait()) == 0){
	    cout << "empty set" << endl;
	    goto check;
	}
        else if(size < 0){
            this-> status = -1;
            cout << "epollWait failed!"<<endl;
            break;
        }
        else{
            cout << "接收到"<< size <<"个新连接" << endl;
            vector<epoll_event>& events = this->IOMultiplexer.getEventList();
            this->IOQueue.Push(events, size);
            for(int i = 0; i < size; ++i){
                if(events[i].data.fd == this->serverSocket){
                    conn_sock.changeSocket(this->serverSocket, SERVER_SOCKET);
                    while((accept_sock=conn_sock.Accept()) > 0){
                        temp = this->IOMultiplexer.setEpollEvent(accept_sock, EPOLLIN | EPOLLET);
                        this->IOMultiplexer.epollAddEvent(temp);
                    }

                    if (errno != EAGAIN && errno != ECONNABORTED && errno != EPROTO && errno != EINTR){
                        this->status = -1;
                        perror("accept error");
                    }
                    break;
                }
            }
        }
    check:
        if(this->status < 0) break;
    }
    cout<< "Acceptor main thread exited!"<<endl;
}

void Acceptor::run()
{
    status = 0;
    std::thread AcceptorThread(&Acceptor::AcceptorCycle, this);
    AcceptorThread.detach();
}
