#ifndef _SOCKET_H_INCLUDED_
#define _SOCKET_H_INCLUDED_
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <errno.h>
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
    static extern int errno;
    static string errMsg;
    Socket(int socket, socketType type);
    int Accept();
    int Send(void* buffer, size_t bytes);
    int Recv(void* buffer, size_t bytes);
    bool Connect();
    bool Close();
    void changeSocket(int socket, socketType type);
    static int initListenSocket(string IPV4_addr = "", int port = 0);
    static int initClientSocket();
    ~Socket();
};

#endif