#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <ctime>
#include <vector>
#include <arpa/inet.h> // inet_ntoa() //将ip地址==> str

// #define PORT 12345


int main(int argc,char *argv[]) {
    //提示输入
    if(argc != 2){
        printf("server start error!, Usage error\n for example: ./test_block_io 12345\n");
        exit(0);
    }
    //创建socket
    int listenfd = socket(AF_INET,SOCK_STREAM,0);
    sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); //绑定本机
    servaddr.sin_port=htons(atoi(argv[1])); //指定端口
    //bind()
    bind(listenfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
    //listen()
    listen(listenfd,20); // sock_fd, 挂起队列的最大长度 【例如客户端连接100，服务器能处理20， 剩下80个放入该队列】
    printf("server start ...\n");

    //select配置
    fd_set readFds;  //监听读(fd_set长度是一定的)
    int maxFd = listenfd; 
    std::vector<int> conns(1024, -1); 
    char buf[1024]; //用于和client通信的缓冲区

    while (true) {
        FD_ZERO(&readFds); //清空read集合
        FD_SET(listenfd, &readFds); //注册事件（监听listen）
        for(int i = 0; i < 1024; ++i){
            if(conns[i] == -1) continue;
            FD_SET(conns[i], &readFds); //注册事件（交流事件）
        }
        
        int readyFds = select(maxFd + 1, &readFds, nullptr, nullptr, nullptr); //监听读
        if (readyFds == -1) {
            std::cerr << "Select error." << std::endl;
            break;
        } else if (readyFds == 0) {
            std::cout << "No activity, waiting..." << std::endl;
            continue;
        }
        if (FD_ISSET(listenfd, &readFds)) {
            sockaddr_in clientAddress{};
            socklen_t clientAddrLen = sizeof(clientAddress);
            int clientSocket = accept(listenfd, reinterpret_cast<sockaddr*>(&clientAddress), &clientAddrLen);
            printf("clientSocket fd = %d", clientSocket);
            if (clientSocket == -1) {
                std::cerr << "Failed to accept connection." << std::endl;
                continue;
            }else{
                for(int i = 0; i < 1024; ++i){
                    if(conns[i] == -1){  //找空位
                        printf("添加一个新的连接： %s", inet_ntoa(clientAddress.sin_addr));
                        conns[i] = clientSocket; //保存该socket
                        maxFd = conns[i] > maxFd ? conns[i]: maxFd;
                        break;
                    }
                }
            }
        }
        //处理通信
        for(int i = 0; i < 1024; ++i){
            int clientSocket = conns[i];
            if(clientSocket == -1) continue; //不可连接
            memset(buf, 0, 1024);
            if(FD_ISSET(clientSocket, &readFds)){//读事件
                //读事件
                int ret = recv(clientSocket, buf, 1024, 0);
                printf("received: %s", buf);
                send(clientSocket, buf, 1024, 0);


            }
        }

        sleep(1);
        
    }
}

