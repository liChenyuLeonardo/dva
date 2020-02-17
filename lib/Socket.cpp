#include "Socket.h"


Socket::Socket(int socket, socketType type):
addrlen(sizeof(sockaddr_in)), socketfd(socket), type(type)
{
}

Socket::Socket():
addrlen(sizeof(sockaddr_in)), socketfd(-1), type(CLIENT_SOCKET)
{
}

int Socket::Accept()
{
    if(socketfd == -1) 
        return -1;
    if(type == CLIENT_SOCKET) 
        return -1;
    return accept(socketfd, (struct sockaddr*)&addr, &addrlen);
}

/*
return: 
    -1: Error happens.
    0 : Connection lost.
    others(>0): The length of received bytes

*/
int Socket::Recv(void* buffer, size_t length)
{
    if(socketfd == -1){ 
        return -1;
    }
    if(type == SERVER_SOCKET){
        return -1;
    }
    size_t bytes_left;
    size_t bytes_read;
    bytes_left = length;
    char* current = (char*)buffer;
    while(bytes_left > 0)
    {
        bytes_read = recv(socketfd,(void*)current,bytes_read, MSG_DONTWAIT); //non-blocking recv()
        if(bytes_read < 0)
        {
            if(errno == EINTR)
                bytes_read = 0;
            else
                return -1;
        }
    else if(bytes_read == 0) break;
    bytes_left -= bytes_read;
    current += bytes_read;
    }
    return length - bytes_left;
}

int Send(void* buffer, size_t bytes)
{
    //to be continue
    return 0;
}

bool Socket::Close()
{
    if(socketfd == -1) return true;
    return close(socketfd) == -1? false : true;
}

void Socket::changeSocket(int socket, socketType type)
{
    this->socketfd = socket;
    this->type = type;
}

sockaddr_in Socket::getAddr()
{
    return addr;
}
/*
flags: 1->Non-blocking
       0->Blocking(default)
*/
int Socket::initListenSocket(string IPV4_addr, int port, int nfds , int flags)
{
    int listen_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(listen_sock < 0) return -1;
    if(flags == 1) setNonblockingSocket(listen_sock);
    sockaddr_in serverSock;
    memset(&serverSock, 0, sizeof(serverSock));
    serverSock.sin_family = AF_INET;
    serverSock.sin_addr.s_addr = inet_addr(IPV4_addr.c_str());
    serverSock.sin_port = htons(port);
    if(bind(listen_sock, (sockaddr*)&serverSock,sizeof(serverSock)) < 0) 
        return -1;
    if(listen(listen_sock,nfds) < 0)
        return -1;
    return listen_sock;	
}

bool Socket::setNonblockingSocket(int socket)
{
    int flags = fcntl(socket, F_GETFL, 0);
    fcntl(socket, F_SETFL, flags|O_NONBLOCK);
}
