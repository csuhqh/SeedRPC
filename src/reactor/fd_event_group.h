#ifndef SRC_REACTOR_FD_EVENT_GROUP_H
#define SRC_REACTOR_FD_EVENT_GROUP_H

#include <vector>
#include "src/common/mutex.h"
#include "src/reactor/fd_event.h"
#include <memory>

namespace seedrpc {
/**
 * 避免FdEvent的频繁创建和销毁对象，构建一个池子存放。
*/
class FdEventGroup {

 public:
  typedef std::shared_ptr<FdEventGroup> s_ptr;
  FdEventGroup(int size);

  FdEvent::s_ptr getFdEvent(int fd);

 public:
  static FdEventGroup::s_ptr GetFdEventGroup();

 private:
  int m_size {0};
  std::vector<FdEvent::s_ptr> m_fd_group;
  Mutex m_mutex; 

};

}

#endif