#ifndef ROCKET_NET_EVENTLOOP_H
#define ROCKET_NET_EVENTLOOP_H

#include <pthread.h>
#include <set>
#include <functional>
#include <queue>
#include "rocket/common/mutex.h"
#include "rocket/net/fd_event.h"
#include "rocket/net/wakeup_fd_event.h"
#include "rocket/net/timer_fd_event.h"

namespace rocket {
class EventLoop {
 public:
  EventLoop();

  ~EventLoop();

  void loop();

  void wakeup(); //唤醒

  void stop(); //终止循环（基本不会被调用）

  void addEpollEvent(FdEvent* event);

  void deleteEpollEvent(FdEvent* event);

  bool isInLoopThread(); //当前线程是不是呢

  void addTask(std::function<void()> cb, bool is_wake_up = false);

  void addTimerTask(TimerTask::s_ptr);

  bool isLooping();

 public:
  static EventLoop* GetCurrentEventLoop();


 private:
  void dealWakeup();

  void initWakeUpFdEevent();

  void initTimer();

 private:
  pid_t m_thread_id {0};  //线程的id

  int m_epoll_fd {0};

  int m_wakeup_fd {0};

  WakeUpFdEvent* m_wakeup_fd_event {NULL};
  TimerFdEvent* m_timer_fd_event {NULL};

  bool m_stop_flag {false};

  std::set<int> m_listen_fds; //当前监听的所有套接字

  std::queue<std::function<void()>> m_pending_tasks;
  
  Mutex m_mutex;

  

  bool m_is_looping {false};

};

}


#endif