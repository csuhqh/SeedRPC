#include<stdio.h>
#include<iostream>
#include<cstring>
#include<stdlib.h>
#include<sys/fcntl.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>
#include<errno.h>
#include<sys/types.h>
#include <arpa/inet.h>
using namespace std;
/**
 * 在accept()阻塞，[一个线程对应一个客户端，在read()处阻塞】
*/

int main(int argc,char *argv[])
{
    if(argc != 2){
        printf("block_io_serve start error!, Usage error\n for example: ./test_block_io 12345\n");
        exit(0);
    }
    //1. 创建socket()
    int listenfd;
    listenfd=socket(AF_INET,SOCK_STREAM,0);// in socket code,it must be AF_INET(protocol) 
    if(listenfd==-1)
    {
        printf("socket create fail\n");
        return -1;
    }
    //2. 绑定本机并监听指定端口 bind(), listen()
    struct sockaddr_in serveraddr;
    memset(&serveraddr,0,sizeof(serveraddr));
    serveraddr.sin_family=AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port=htons(atoi(argv[1])); //指定端口
    if(bind(listenfd,(struct sockaddr *)&serveraddr,sizeof(serveraddr))!=0)
    {
        printf("bind failed \n");
        return -1;
    }
    if(listen(listenfd,5)!=0)
    {
        printf("Listen failed\n");
        close(listenfd);
        return -1;
    }

    //3. 阻塞并等待客户端的连接
    int clintfd;//socket for client
    int socklen=sizeof(struct sockaddr_in);
    struct sockaddr_in client_addr;
    clintfd=accept(listenfd,(struct sockaddr*)&client_addr,(socklen_t *)&socklen);
    if(clintfd==-1)
        printf("connect failed\n");
    else
        printf("client %s has connnected\n",inet_ntoa(client_addr.sin_addr));
   
    //4. 与客户端通信 （这里可以用线程替代）
    char buffer[1024];
    while (1)
    {
    int iret;
    memset(buffer,0,sizeof(buffer));
    printf("阻塞等待客户端消息...");
    iret=recv(clintfd,buffer,sizeof(buffer),0);
    if (iret<=0) 
    {
       printf("iret=%d\n",iret); break;  
    }
    printf("receive :%s\n",buffer);
 
    if ( (iret=send(clintfd,buffer,sizeof(buffer),0))<=0) 
    { 
        perror("send"); 
        break; 
    }
    printf("send :%s\n",buffer);
  }
    // 6th close socket
    close(listenfd); close(clintfd);
}
