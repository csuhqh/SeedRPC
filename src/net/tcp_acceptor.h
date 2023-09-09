#ifndef SRC_NET_TCP_ACCEPTOR_H
#define SRC_NET_TCP_ACCEPTOR_H

#include <memory>
#include "src/net/net_addr.h"

namespace seedrpc {

class TcpAcceptor {
 public:
  typedef std::shared_ptr<TcpAcceptor> s_ptr;

  TcpAcceptor(NetAddr::s_ptr local_addr);


  //返回连接的  connfd, 地址
  std::pair<int, NetAddr::s_ptr> accept(); 
  //服务器端的套接字
  int getListenFd(); 

 private:
  NetAddr::s_ptr m_local_addr; // 服务端监听的地址，addr -> ip:port 

  int m_family {-1};

  int m_listenfd {-1}; // 监听套接字

};

}

#endif