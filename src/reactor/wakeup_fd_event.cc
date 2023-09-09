#include <unistd.h>
#include "src/reactor/wakeup_fd_event.h"
#include "src/common/log.h"
#include <sys/eventfd.h>

namespace seedrpc {

WakeUpFdEvent::WakeUpFdEvent(){
  m_fd = eventfd(0, EFD_NONBLOCK);
  if (m_fd < 0) {
    ERRORLOG("failed to create wakeup fd, eventfd create error, error info[%d]", errno);
    exit(0);
  }
  INFOLOG("wakeup fd = %d", m_fd);

  listen(FdEvent::IN_EVENT, [this]() {
    char buf[8];
    while(read(m_fd, buf, 8) != -1 && errno != EAGAIN) {
    }
    DEBUGLOG("read full bytes from wakeup fd[%d]", m_fd);
  });
}


void WakeUpFdEvent::wakeup() {
  char buf[8] = {'a'};
  int rt = write(m_fd, buf, 8); //写入8个字节的数据
  if (rt != 8) {
    ERRORLOG("write to wakeup fd less than 8 bytes, fd[%d]", m_fd);
  }
  DEBUGLOG("success read 8 bytes");
}


}