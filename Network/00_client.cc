#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include<string>
#include<iostream>
/**
 * 一个简单的客户端，每次发送1024字节消息给服务器。并将服务器返回的1024个字节消息打印。
 * send: hello
 * receivel: hello
*/
using namespace std;
int main(int argc,char *argv[]) //argc为参数个数， char *argv[]
{
  if(argc != 3){
    printf("Useage error;\nfor example: ./client 127.0.0.1 12345\n");
    exit(0);
  } 
  // 1. 创建套接字socket()
  int sockfd;
  if ( (sockfd = socket(AF_INET,SOCK_STREAM,0)) == -1 ) 
  { 
      perror("socket"); 
      return -1; 
  }
 
  // 2. 与服务器建立连接connect()
  struct sockaddr_in servaddr;
  memset(&servaddr,0,sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(atoi(argv[2])); // server's port 12345
  servaddr.sin_addr.s_addr=inet_addr(argv[1]);//server's ip 127.0.0.1
  if (connect(sockfd, (struct sockaddr *)&servaddr,sizeof(servaddr)) != 0)  // send request to server for connection
  { 
      perror("connect"); 
      close(sockfd); 
      return -1; 
  }

  //3. 与服务器通信
  char buffer[1024];
  printf("Talking with Server in English:\n");
  while(1)
  {
    int iret;
    memset(buffer,0,sizeof(buffer));
    cin.getline(buffer,1024); 
    if ( (iret=send(sockfd,buffer,strlen(buffer),0))<=0) // send data to server
    { 
        perror("send"); 
        break; 
    }
    printf("send: %s\n",buffer);
 
    memset(buffer,0,sizeof(buffer));
    if ( (iret=recv(sockfd,buffer,sizeof(buffer),0))<=0) // receive server's reply
    {
       printf("iret=%d\n",iret); 
       break;
    }
    printf("receive: %s\n",buffer);
  }
  close(sockfd);
}
