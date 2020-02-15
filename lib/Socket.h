#ifndef _SOCKET_H_INCLUDED_
#define _SOCKET_H_INCLUDED_
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <string>
using std::string;


enum socketType{
    CLIENT_SOCKET,SERVER_SOCKET
};

//The creation and initialization of an IPV4 socket
class Socket
{
private:
    int socketfd;
    socketType type;
    socklen_t addrlen;
    sockaddr_in addr;
    /*---------------------------*/

public:
    Socket();
    int errno;
    Socket(int socket, socketType type);
    int Accept();
    int Send(void* buffer, size_t bytes);
    int Recv(void* buffer, size_t bytes);
    bool Close();
    void changeSocket(int socket, socketType type);
    sockaddr_in getAddr();
    //用于初始化一个侦听socket
    static int initListenSocket(string IPV4_addr = "", int port = -1, int nfds = 0);
    //static int initClientSocket();
    ~Socket() = default;
};

#endif