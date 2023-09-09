#ifndef SRC_NET_TCP_SERVER_H
#define SRC_NET_TCP_SERVER_H

#include <set>
#include "src/net/tcp_acceptor.h"
#include "src/net/net_addr.h"
#include "src/reactor/eventloop.h"
#include "src/reactor/io_thread_group.h"
#include "src/net/tcp_connection.h"
#include <memory>

namespace seedrpc{

class TcpServer{
  public:
    typedef std::shared_ptr<TcpServer> s_ptr;
    TcpServer(NetAddr::s_ptr local_addr);
    void start();
  private:
    void init();
    //有客户端连接
    void onAccept(); 
    //清除closed的连接
    void clearClientTimerFunc();

  private:
    TcpAcceptor::s_ptr m_acceptor;
    NetAddr::s_ptr m_local_addr; //本地监听地址
    std::shared_ptr<EventLoop> m_main_event_loop;  //主Rector
    std::shared_ptr<IOThreadGroup> m_io_threads; //IO线程池  subRectors
    std::shared_ptr<FdEvent> m_listen_fd_event;
    int m_client_couts{0}; //连接的客户端数量
    std::set<TcpConnection::s_ptr> m_clients; //存放每个与客户端的连接
    TimerTask::s_ptr m_clear_client_timertask;
};
}

#endif