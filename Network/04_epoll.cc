/**
一个epoch服务器，使用epoch实现。（单纯的epoch服务器）
*/
#include<stdio.h>
#include<arpa/inet.h>
#include<sys/epoll.h>
#include<unistd.h>
#include<ctype.h>
#include <iostream>
#define SERV_PORT 8000
#define MAX_OPEN_FD 1024

int main(int argc,char *argv[])
{
    if(argc != 2){
        printf("server start error!, Usage error\n for example: ./test_block_io 12345\n");
        exit(0);
    }
    int  listenfd, connfd, efd, ret; //服务器端socket套接字，建立连接的套接字，epoll句柄，
    char buf[1024]; //用于和client通信的缓冲区
    struct sockaddr_in cliaddr,servaddr; //客户端地址结构体，服务端地址结构体
    socklen_t clilen = sizeof(cliaddr);


    //创建服务器端socket，并监听12345端口
    listenfd = socket(AF_INET,SOCK_STREAM,0);
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port=htons(atoi(argv[1])); //指定端口
    bind(listenfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
    listen(listenfd,20); // sock_fd, 挂起队列的最大长度 【例如客户端连接100，服务器能处理20， 剩下80个放入该队列】


    // 创建一个epoll实例
    efd = epoll_create(MAX_OPEN_FD);
    struct epoll_event tep,ep[MAX_OPEN_FD]; //epoll_event = ｛ int events; epoll_data_t data｝
    tep.events = EPOLLIN; //事件的类型， EPOLLIN, EPOLLOUT, EPOLLERR
    tep.data.fd = listenfd; //data.fd 存放监听的文件描述符（也可以不写。）
    // 把监听socket 先添加到efd中
    ret = epoll_ctl(efd, EPOLL_CTL_ADD, listenfd, &tep); //epoll_ctl注册epoll监听的事件
    std::cout << "server start at:" << std::endl;
    // 循环等待
    while(1)
    {   std::cout << "server epoll_waiting" << std::endl;
        // 返回已就绪的epoll_event,-1表示阻塞,没有就绪的epoll_event,将一直等待
        size_t nready = epoll_wait(efd ,ep ,MAX_OPEN_FD ,-1);
        // continue;
        for (int i = 0; i < nready; ++i)
        {
            // 如果是新的连接,需要把新的socket添加到efd中
            if (ep[i].data.fd == listenfd ) //如果是服务器这个socket
            {
                connfd = accept(listenfd,(struct sockaddr*)&cliaddr,&clilen);
                tep.events = EPOLLIN;
                tep.data.fd = connfd;
                ret = epoll_ctl(efd,EPOLL_CTL_ADD,connfd,&tep);
            }
            // 否则,读取数据 [可以放到线程中执行]
            else
            {
                connfd = ep[i].data.fd;
                int ret = read(connfd,buf,1024);
                // 客户端关闭连接
                if (ret == 0){
                    ret =epoll_ctl(efd, EPOLL_CTL_DEL,connfd,NULL);
                    close(connfd);
                    printf("client[%d] closed\n", i);
                }
                else
                {
                    // 向客户端发送数据
                    write(connfd,buf,1024);
                }
            }
        }
    }
    return 0;
}