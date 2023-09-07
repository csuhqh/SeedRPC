#ifndef SRC_NET_WAKEUP_FDEVENT_H
#define SRC_NET_WAKEUP_FDEVENT_H

#include "src/net/fd_event.h"
#include <memory>
namespace seedrpc {

class WakeUpFdEvent : public FdEvent {
 public:
  typedef std::shared_ptr<WakeUpFdEvent> s_ptr;
  WakeUpFdEvent();
  void wakeup();

 private:

};



}

#endif