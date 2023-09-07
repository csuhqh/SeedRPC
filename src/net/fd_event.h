
#ifndef SRC_NET_FDEVENT_H
#define SRC_NET_FDEVENT_H

#include <functional>
#include <sys/epoll.h>
#include <memory>
#include "src/common/mutex.h"

namespace seedrpc {
class FdEvent{
 public:
  enum TriggerEvent {
    IN_EVENT = EPOLLIN,
    OUT_EVENT = EPOLLOUT,
    ERROR_EVENT = EPOLLERR,
  };
  typedef std::shared_ptr<FdEvent> s_ptr;
  FdEvent(int fd);

  FdEvent();


  void setNonBlock();

  std::function<void()> handler(TriggerEvent event_type);

  void listen(TriggerEvent event_type, std::function<void()> callback, std::function<void()> error_callback = nullptr);

  // 取消监听
  void cancle(TriggerEvent event_type);

  int getFd() const {
    return m_fd;
  }

  epoll_event getEpollEvent() {
    return m_listen_events;
  }

 uint32_t getId(){
    return m_id;
 }

 protected:
  static uint32_t generateId(){ //每个Fd_event有唯一id
    ScopeMutex<Mutex> lock(mutex);
    ++id;
    return id;
  }

  static int id;
  static Mutex mutex;

  int m_id{-1}; //fd_event对应的id
  int m_fd {-1}; //fd_event对应的文件描述符

  epoll_event m_listen_events;
  std::function<void()> m_read_callback {nullptr};
  std::function<void()> m_write_callback {nullptr};
  std::function<void()> m_error_callback {nullptr};

};

}

#endif