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
#include <vector>
using namespace std;
/**
 * 非阻塞IO，可以通过保存所有建立连接的fd, 以及服务器的fd到一个列表。不断轮询是否有事件
 * 发生。
*/

//设置指定文件描述符为非阻塞
bool setNotBlocking(int fd){
  int flags = fcntl(fd, F_GETFL, 0); // 获取给定文件描述符的当前标志位
  if(flags == -1){
    printf("Failed to get file descriptor flags...");
    return false;
  }
  if(fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1){
    printf("Failed to set non-blocking mode");
    return false;
  }
  return true;
}

int main(int argc,char *argv[])
{
    std::vector<int> conns;
    std::vector<int> disconns; //存放断开连接的
    std::vector<int> errorconns; //存放发送错误的连接
    if(argc != 2){
        printf("block_io_server start error!, Usage error\n for example: ./test_block_io 12345\n");
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

    //设置服务器监听连接的为非阻塞
    setNotBlocking(listenfd);

    //3. 阻塞并等待客户端的连接
    int clientfd;//socket for client
    int socklen=sizeof(struct sockaddr_in);
    struct sockaddr_in client_addr;

    char buffer[1024]; //用于和客户端通信

    int ori;
    //轮询
    while(true){
      //轮询是否有客户端连接
      clientfd=accept(listenfd,(struct sockaddr*)&client_addr,(socklen_t *)&socklen);
      if(clientfd == -1){ //轮询是否有客户端连接
        printf("没有客户端连接, 继续等待.\n");
      }else{ //有连接
        setNotBlocking(clientfd);
        conns.push_back(clientfd);
        

      }

      for(int i = 0; i < conns.size(); ++i){
        int conn = conns[i];
        memset(buffer,0,sizeof(buffer));
        int ret = recv(conns[0], buffer, sizeof(buffer), 0);
        if(ret == 0){  //连接断开
          printf("客户端[%d]断开连接\n", conn);
          disconns.push_back(i);
        }else if(ret > 0){  //正常通信  [这里可以用线程。但是cpu依旧在忙碌]
          printf("收到客户端[%d]消息:%s, 并回复相同消息", conn, buffer);
          if ( (send(conn,buffer,sizeof(buffer),0)) < 0) { //epoch
            errorconns.push_back(i);
          }
        }else{  //等待消息
          printf("等待客户端[%d]消息...\n", conn);
        }
        // printf("ret=%d\n", ret);  
      }
      //删除发生错误的连接和断开的连接
      for(int i: disconns){
        conns.erase(conns.begin() + i);
      }
      for(int i: errorconns){
        conns.erase(conns.begin() + i);
      }
      sleep(1);
    }
    
    
    if(clientfd==-1)
        printf("connect failed\n");
    else
        printf("client %s has connnected\n",inet_ntoa(client_addr.sin_addr));
   
    //4. 与客户端通信
    
    while (1)
    {
    int iret;
    memset(buffer,0,sizeof(buffer));
    iret=recv(clientfd,buffer,sizeof(buffer),0);
    if (iret<=0) 
    {
       printf("iret=%d\n",iret); break;  
    }
    printf("receive :%s\n",buffer);
 
    if ( (iret=send(clientfd,buffer,strlen(buffer),0))<=0) 
    { 
        perror("send"); 
        break; 
    }
    printf("send :%s\n",buffer);
  }
    // 6th close socket
    close(listenfd); close(clientfd);
}
