#include "Acceptor.h"

Acceptor::Acceptor(epollEngine& ep, doubleBufferedQueue<event_t>& q, int s):
IOMultiplexer(ep), IOQueue(q), serverSocket(s), status(0)
{

}

void Acceptor::AcceptorCycle()
{
    int size = 0;
    Socket conn_sock;
    int accept_sock = -1;
    while(true){
        if((size=IOMultiplexer.epollWait()) == 0) continue;
        else if(size < 0){
            status = -1;
            //log: IOMultiplexer exit with error
            break;
        }
        else{
            vector<epoll_event>& events = IOMultiplexer.getEventList();
            IOQueue.Push(events, size);
            for(auto& val : events){
                if(val.data.fd == serverSocket){
                    conn_sock.changeSocket(serverSocket, SERVER_SOCKET);
                    while((accept_sock=conn_sock.Accept()) > 0){
                        IOMultiplexer.add
                    }
                }
            }

        }
    }
    
}