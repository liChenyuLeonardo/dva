#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>
#include <string>
using namespace std;
int main(){
    for(int i = 0; i < 5; i++){
    //创建套接字
    fork();
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    //向服务器（特定的IP和端口）发起请求
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));  //每个字节都用0填充
    serv_addr.sin_family = AF_INET;  //使用IPv4地址
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  //具体的IP地址
    serv_addr.sin_port = htons(1234);  //端口
        if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1){
            perror("connection failed");
            break;
        }
	cout << "Process "<<getpid()<<" successfully established connection!"<<endl;
	string msg = "hello from ";
	msg += (int)getpid();
	for(int j = 0; j < 10; j++){
	    if(send(sock, msg.c_str(), (int)msg.size(), 0) <= 0){
	    	perror("send msg error");
	    }
	    sleep(1);
	}
        //关闭套接字
        close(sock);
    }
    return 0;
}
