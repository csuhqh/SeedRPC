#ifndef SRC_NET_EVENTLOOP_H
#define SRC_NET_EVENTLOOP_H

#include <pthread.h>
#include <unordered_map>
#include <functional>
#include <queue>
#include <set>
#include "src/common/mutex.h"
#include "src/net/fd_event.h"
#include "src/net/wakeup_fd_event.h"
#include "src/net/timer_fd_event.h"
#include <memory>

namespace seedrpc {
class EventLoop {
 public:
  typedef std::shared_ptr<EventLoop> s_ptr;
  EventLoop();
  void loop();
  void wakeup(); //唤醒

  void stop(); //终止循环（基本不会被调用）

  void addEpollEvent(FdEvent::s_ptr event);

  void deleteEpollEvent(FdEvent::s_ptr event);

  bool isInLoopThread(); //当前线程是不是呢

  void addTask(std::function<void()> cb, bool is_wake_up = false);

  void addTimerTask(TimerTask::s_ptr);

  bool isLooping();

 private:
  void dealWakeup();

  void initWakeUpFdEevent();

  void initTimer();

 private:
  //在epoll上注册事件
  bool registerFdEvent(FdEvent::s_ptr event);
  //在epoll上解绑事件
  bool unregisterFdEvent(FdEvent::s_ptr event);

  pid_t m_thread_id {0};  //线程的id

  int m_epoll_fd {0};

  int m_wakeup_fd {0};

  WakeUpFdEvent::s_ptr m_wakeup_fd_event;
  TimerFdEvent::s_ptr m_timer_fd_event;
  std::unordered_map<uint32_t, FdEvent::s_ptr> m_listened_event;// id, FdEvent::s_ptre

  bool m_stop_flag {false};

  std::set<int> m_listen_fds; //当前监听的所有套接字

  std::queue<std::function<void()>> m_pending_tasks;
  
  Mutex m_mutex;

  

  bool m_is_looping {false};

};

}


#endif