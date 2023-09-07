# SeedRPC


## 整体说明
这是由C++11开发的高性能RPC库，代码在Ubuntu18.04上远程开发, 依赖的库(jsoncpp, protobuf)也包含在其中。
- build：项目构建目录。
- mylibs: 项目依赖的库（已经编译），方便复现
- test：用于测试项目的源码文件。命名text_xx.cc。
## 项目构造: 
cmake构建，项目依赖的库都已经配置好了，可以直接运行。
## 如何运行: 
```
cd build //进入某个项目的build目录
cmake ..  //生成makefile
make  //编译
./test_xxx  //执行对于测试文件
```

## 项目: Rocket 高并发RPC框架
参考：
https://github.com/Gooddbird/seedrpc

## 项目组成
将文件描述符 ==> 封装为Fd_event对象 ==> 并在epoll中进行注册。
## 环境搭建
开发环境：Vscode + 远程Ubuntu。
依赖库(已安装): jsoncpp

### 日志模块


### 配置模块
读取JSON文件作为项目的配置模块
### EventLoop模块


#### 定时器:
   Timer_fd_event: 使用timerfd，在epoll注册事件，回调OnTimer()方法。
   Timer_Task: OnTimer()中回调所有符合时间的任务。

1. 既然epoll_event.data.ptr区域只能存放void*原始指针，因此，所有fd_event需要用原始指针。





