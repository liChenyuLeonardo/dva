#include "Socket.h"
string Socket::errMsg = "Everything is OK.";


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
    
}