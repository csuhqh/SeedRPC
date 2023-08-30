## 项目介绍
客户端与服务器通信的**epoch服务器**，就是客户端发什么，服务器回应什么。

客户端：
- 一个由普通socket构建的客户端

服务器端: 
- 利用不同的技术构建，传统IO(bio/nbio)与IO复用(select/epoll)构建

## 文件介绍
- client.cc : 
```
/**
 * 一个简单的客户端，每次发送1024字节消息给服务器。并将服务器返回的1024个字节消息打印。
 * send: hello
 * receivel: hello
*/
```
- bio.cc:
```
传统阻塞IO，socket(), bind(), listen(), accept(). 【recv(), send()】 线程阻塞 
```
- nio.cc:
```
非阻塞IO, 轮询。用一个线程去轮询所有fd, cpu一直处于忙碌状态。
```
- epoll.cc
```
IO多路复用之Epoll
```