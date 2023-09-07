#ifndef SRC_NET_CLIENT_TCP_CLIENT_H
#define SRC_NET_CLIENT_TCP_CLIENT_H

#include <memory>
#include "src/net/tcp/net_addr.h"
#include "src/net/eventloop.h"
#include "src/net/tcp/tcp_connection.h"
#include "src/net/rpc/coder/abstract_protocol.h"
#include "src/net/timer_task.h"


namespace seedrpc{
/**
 * 主要是采用异步io
*/

class TcpClient{
  public:
    typedef std::shared_ptr<TcpClient> s_ptr;
    TcpClient(NetAddr::s_ptr peer_addr);
    //异步connect，如果connect完成，done会执行
    void connect(std::function<void()> done);
    //message发送成功，会调用done
    void writeMessage(AbstractProtocol::s_ptr message, std::function<void(AbstractProtocol::s_ptr)> done);
    //如果message读取成功，会调用done
    void readMessage(const std::string& msg_id, std::function<void(AbstractProtocol::s_ptr)> done);

    void stop();

    int getConnectErrorCode();
    std::string getConnectErrorInfo();

    NetAddr::s_ptr getPeerAddr();

    NetAddr::s_ptr getLocalAddr();
    void initLocalAddr();
    void addTimerTask(TimerTask::s_ptr timer_task);

  private:
    NetAddr::s_ptr m_peer_addr;
    NetAddr::s_ptr m_local_addr;
    EventLoop::s_ptr m_event_loop;
    int m_fd;
    FdEvent::s_ptr m_fd_event;
    TcpConnection::s_ptr m_connection;
    int m_connect_error_code{0};
    std::string m_connect_error_info;
};
  
}

#endif