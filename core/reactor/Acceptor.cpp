#include "Acceptor.h"

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
    if(serverSocket == -1){
        //log: invalid serverSocket
        return;
    }
    temp = IOMultiplexer.setEpollEvent(serverSocket, EPOLLIN | EPOLLET);
    IOMultiplexer.epollAddEvent(temp); //将监听socket添加入epoll中，监听socket设为ET
    //main circle
    while(true){
        if((size=IOMultiplexer.epollWait()) == 0) goto check;
        else if(size < 0){
            status = -1;
            //log: IOMultiplexer exit with error, perror()
            break;
        }
        else{
            vector<epoll_event>& events = IOMultiplexer.getEventList();
            IOQueue.Push(events, size);
            for(auto& val : events){
                if(val.data.fd == serverSocket){
                    conn_sock.changeSocket(serverSocket, SERVER_SOCKET);
                    while((accept_sock=conn_sock.Accept()) > 0){
                        temp = IOMultiplexer.setEpollEvent(accept_sock, EPOLLIN | EPOLLET);
                        IOMultiplexer.epollAddEvent(temp);
                    }

                    if (errno != EAGAIN && errno != ECONNABORTED && errno != EPROTO && errno != EINTR){
                        status = -1;
                        //log: Accept Error, perror()
                    }
                    break;
                }
            }
        }
    check:
        if(status < 0) break;
    }
    
}

void Acceptor::run()
{
    status = 0;
    std::thread AcceptorThread(&Acceptor::AcceptorCycle, this);
    AcceptorThread.detach();
}