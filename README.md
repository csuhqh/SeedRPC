# SeedRPC
这是由C++11开发的**多线程基于epoll实现的RPC框架**，代码编译管理工具为Cmake在Ubuntu18.04上远程开发, 依赖的库(jsoncpp, protobuf)也包含在mylibs其中，可以下载后，在src同级目录下,安装以下方式运行。(以后也会不断完善...)
```
cd build
cmake ..
make
./test_rpc_server
./test_rpc_client  //再另一个终端
```

## 技术要点:
- 多线程知识 (pthread)
- C++11语法
- 主从Reactor模型 (epoll)
- timer (定时器)
- 设计模式 (单例模式)
- 线程间通信 （锁/信号量）
- rpc通信原理， Protobuf 协议
- 计算机网络， Tcp知识。

## 环境搭建
开发环境：Vscode + 远程Ubuntu18.04（使用VMWare搭建）。

依赖库(已安装): jsoncpp1.9.2, protobuf3.16.3

构建工具：CMake 3.10.2
## 项目组成
### 日志模块
可以实现异步日志，打印输出日志所在文件和行号，支持多个级别日志打印。
### 配置模块
使用json作为我们的配置文件
### Reactor模块
一个主线程执行loop循环，一个线程池作为从Reactor，主线程只负责接受客户端请求，线程池中根据连接socket_fd与客户端连接通信。
### RPC模块
使用Protobuf作为序列化工具。
### 测试模块
测试对各个模块的测试






