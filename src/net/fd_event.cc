#include <string.h>
#include <fcntl.h>
#include "src/net/fd_event.h"
#include "src/common/log.h"
#include "src/common/mutex.h"

namespace seedrpc {

int FdEvent::id = 0;
Mutex FdEvent::mutex;


FdEvent::FdEvent(int fd) : m_fd(fd) {
  m_id = generateId();
  memset(&m_listen_events, 0, sizeof(m_listen_events));
}


FdEvent::FdEvent() {
  m_id = generateId();
  memset(&m_listen_events, 0, sizeof(m_listen_events));
}





std::function<void()> FdEvent::handler(TriggerEvent event) {
  if (event == TriggerEvent::IN_EVENT) {
    return m_read_callback;
  } else if (event == TriggerEvent::OUT_EVENT) {
    return m_write_callback;
  } else if (event == TriggerEvent::ERROR_EVENT) {
    return m_error_callback;
  }
  return nullptr;
}

//绑定事件（append方式）
void FdEvent::listen(TriggerEvent event_type, std::function<void()> callback, std::function<void()> error_callback /*= nullptr*/) {
  if (event_type == TriggerEvent::IN_EVENT) {
    m_listen_events.events |= EPOLLIN;
    m_read_callback = callback;
  } else {
    m_listen_events.events |= EPOLLOUT;
    m_write_callback = callback;
  }

  if (m_error_callback == nullptr) {
    m_error_callback = error_callback;
  } else {
    m_error_callback = nullptr;
  }

  // m_listen_events.data.ptr = this; //
    m_listen_events.data.u32 = m_id;
}


void FdEvent::cancle(TriggerEvent event_type) {
  if (event_type == TriggerEvent::IN_EVENT) {
    m_listen_events.events &= (~EPOLLIN);
  } else {
    m_listen_events.events &= (~EPOLLOUT);
  }
}


void FdEvent::setNonBlock() {
  
  int flag = fcntl(m_fd, F_GETFL, 0);
  if (flag & O_NONBLOCK) {
    return;
  }

  fcntl(m_fd, F_SETFL, flag | O_NONBLOCK); //添加非阻塞设置
}


}