#include "src/net/fd_event_group.h"
#include "src/common/mutex.h"
#include "src/common/log.h"

namespace seedrpc {


static FdEventGroup::s_ptr g_fd_event_group = std::make_shared<FdEventGroup>(128);

FdEventGroup::s_ptr FdEventGroup::GetFdEventGroup() {
  return g_fd_event_group;
}

FdEventGroup::FdEventGroup(int size) :m_size(size) {
  for (int i = 0; i < m_size; i++) {
    FdEvent::s_ptr s = std::make_shared<FdEvent>(i);
    m_fd_group.push_back(s);
  }
}


FdEvent::s_ptr FdEventGroup::getFdEvent(int fd) {
  ScopeMutex<Mutex> lock(m_mutex);
  if ((size_t) fd < m_fd_group.size()) {
    return m_fd_group[fd];
  }

  int new_size = int(fd * 1.5);
  for (int i = m_fd_group.size(); i < new_size; ++i) {
    m_fd_group.push_back(std::make_shared<FdEvent>(i));
  }
  return m_fd_group[fd];
}

}