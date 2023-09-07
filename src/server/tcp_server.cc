#include "src/server/tcp_server.h"
#include "src/common/log.h"
#include "src/net/eventloop.h"
#include "src/net/tcp/tcp_connection.h"

namespace seedrpc {

TcpServer::TcpServer(NetAddr::s_ptr local_addr):m_local_addr(local_addr){
  init();
}
void TcpServer::start() {
  INFOLOG("server start: %s", m_local_addr->toString().c_str());
  m_io_threads->start(); //开启线程池线程
  INFOLOG("server's io threads start: %s", m_local_addr->toString().c_str());
  m_main_event_loop->loop(); //开启自身
}
void TcpServer::init(){
  m_acceptor = std::make_shared<TcpAcceptor>(m_local_addr); //封装了对服务器socket
  m_main_event_loop = std::make_shared<EventLoop>(); //获取主eventloop
  INFOLOG("subRector = %d", 2);
  m_io_threads = std::make_shared<IOThreadGroup>(2); //线程池，每一个线程都是一个eventloop
  //绑定socketed到主event_loop()
  int serverfd = m_acceptor->getListenFd();
  m_listen_fd_event = std::make_shared<FdEvent>(serverfd);
  m_listen_fd_event->listen(FdEvent::IN_EVENT, std::bind(&TcpServer::onAccept, this));
  m_main_event_loop->addEpollEvent(m_listen_fd_event);
  //添加定时任务。
  m_clear_client_timertask = std::make_shared<TimerTask>(5000, true, std::bind(&TcpServer::clearClientTimerFunc, this));
  m_main_event_loop->addTimerTask(m_clear_client_timertask);
}

// 有客户端连接(epoll 回调)
void TcpServer::onAccept(){
  auto re = m_acceptor->accept(); //获取 connfd, peer_addr
  int clientfd = re.first;
  NetAddr::s_ptr peer_addr = re.second;
  m_client_couts++;
  //把clientfd添加到任意IO线程中
  IOThread::s_ptr io_thread = m_io_threads->getIOThread(); //随便获取一条io线程
  //rpc_connect
  // TcpConnection::s_ptr connection = std::make_shared<RpcConnection>(io_thread->getEventLoop(), clientfd, 128, peer_addr, m_local_addr);
  //echo
  TcpConnection::s_ptr connection = std::make_shared<TcpConnection>(io_thread->getEventLoop(), clientfd, 128, peer_addr, m_local_addr);
  connection->setState(Connected);
  m_clients.insert(connection);
  INFOLOG("TcpServer succ get client, fd=%d", clientfd);
}
// 清除closed的连接
void TcpServer::clearClientTimerFunc(){
  auto it = m_clients.begin();
  for (it = m_clients.begin(); it != m_clients.end(); ) {
    // TcpConnection::ptr s_conn = i.second;
    if ((*it) != nullptr && (*it).use_count() > 0 && (*it)->getState() == Closed) {
      // need to delete TcpConnection
      DEBUGLOG("TcpConection [fd:%d] will delete, state=%d", (*it)->getFd(), (*it)->getState());
      it = m_clients.erase(it);
    } else {
      it++;
    }

  }

}
}  // namespace seedrpc