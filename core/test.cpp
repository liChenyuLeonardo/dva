#include "epollEngine.hpp"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>
#include<string.h>
using namespace std;
int main()
{
    int listen_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(listen_sock == -1){
        perror("socket error");
        exit(0);
    }
    sockaddr_in serverSock;
    memset(&serverSock, 0, sizeof(serverSock));
    serverSock.sin_family = AF_INET;
    serverSock.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverSock.sin_port = htons(2345);
    bind(listen_sock, (sockaddr*)&serverSock,sizeof(serverSock));
    listen(listen_sock,100); 
    epollEngine test(listen_sock);
    while(1){
        int nfds = test.epollWait();
        if(nfds == -1){
            perror("epoll wait");
        }
        for (int n = 0; n < nfds; ++n) {
            vector<epoll_event>& events = test.getEventList();
            if (events[n].data.fd == listen_sock) {
                 sockaddr_in addr;
                socklen_t addrlen = sizeof(addr);
                int conn_sock = accept(listen_sock, (struct sockaddr *) &addr, &addrlen);
                if (conn_sock == -1) {
                    perror("accept");
            .        exit(EXIT_FAILURE);
                }
                ioctl(conn_sock, FIONBIO, 1);
                struct epoll_event ev = test.setEpollEvent(conn_sock, EPOLLIN | EPOLLET);
                test.epollAddEvent(ev);
             }else {
                       cout << "接受到客户端连接，fd为"<< events[n].data.fd<<endl;
                       close(events[n].data.fd);
                 }
            }    
    }
}