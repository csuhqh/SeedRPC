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