#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <string.h>
#include "rocket/net/eventloop.h"
#include "rocket/common/log.h"
#include "rocket/common/util.h"
#include "rocket/net/timer_fd_event.h"

#define ADD_TO_EPOLL() \
    auto it = m_listen_fds.find(event->getFd()); \
    int op = EPOLL_CTL_ADD; \
    if (it != m_listen_fds.end()) { \
      op = EPOLL_CTL_MOD; \
    } \
    epoll_event tmp = event->getEpollEvent(); \
    INFOLOG("epoll_event.events = %d", (int)tmp.events); \
    int rt = epoll_ctl(m_epoll_fd, op, event->getFd(), &tmp); \
    if (rt == -1) { \
      ERRORLOG("failed epoll_ctl when add fd, errno=%d, error=%s", errno, strerror(errno)); \
    } \
    m_listen_fds.insert(event->getFd()); \
    DEBUGLOG("add event success, fd[%d]", event->getFd()) \


#define DELETE_TO_EPOLL() \
    auto it = m_listen_fds.find(event->getFd()); \
    if (it == m_listen_fds.end()) { \
      return; \
    } \
    int op = EPOLL_CTL_DEL; \
    epoll_event tmp = event->getEpollEvent(); \
    int rt = epoll_ctl(m_epoll_fd, op, event->getFd(), NULL); \
    if (rt == -1) { \
      ERRORLOG("failed epoll_ctl when add fd, errno=%d, error=%s", errno, strerror(errno)); \
    } \
    m_listen_fds.erase(event->getFd()); \
    DEBUGLOG("delete event success, fd[%d]", event->getFd()); \

namespace rocket {

static thread_local EventLoop* t_current_eventloop = NULL;
static int g_epoll_max_timeout = 10000;
static int g_epoll_max_events = 10;

EventLoop::EventLoop() {
  if (t_current_eventloop != NULL) {
    ERRORLOG("failed to create event loop, this thread has created event loop");
    exit(0);
  }
  m_thread_id = getThreadId();
  m_epoll_fd = epoll_create(10);

  if (m_epoll_fd == -1) {
    ERRORLOG("failed to create event loop, epoll_create error, error info[%d]", errno);
    exit(0);
  }

  initWakeUpFdEevent(); //添加唤醒 [监控一个eventfd的写事件，如果要主动唤醒epoll，对它进行写入]
  initTimer(); //添加定时器 [添加一个timer的文件描述符]

  INFOLOG("succ create event loop in thread %d", m_thread_id);
  t_current_eventloop = this;
}

EventLoop::~EventLoop() {
  close(m_epoll_fd);
  if (m_wakeup_fd_event) {
    delete m_wakeup_fd_event;
    m_wakeup_fd_event = NULL;
  }
  if (m_timer_fd_event) {
    delete m_timer_fd_event;
    m_timer_fd_event = NULL;
  }
}


void EventLoop::initTimer() {
  m_timer_fd_event = new TimerFdEvent();
  addEpollEvent(m_timer_fd_event);
}


void EventLoop::initWakeUpFdEevent() {

  m_wakeup_fd_event = new WakeUpFdEvent();
  addEpollEvent(m_wakeup_fd_event);

}

void EventLoop::addTimerTask(TimerTask::s_ptr p) {
  m_timer_fd_event->addTimerTask(std::move(p));
}


void EventLoop::loop() {
  m_is_looping = true;
  while(!m_stop_flag) {
    ScopeMutex<Mutex> lock(m_mutex); 
    std::queue<std::function<void()>> tmp_tasks; 
    m_pending_tasks.swap(tmp_tasks); 
    lock.unlock();

    while (!tmp_tasks.empty()) {
      std::function<void()> cb = tmp_tasks.front();
      tmp_tasks.pop();
      if (cb) {
        cb();
      }
    }

    // 如果有定时任务需要执行，那么执行
    // 1. 怎么判断一个定时任务需要执行？ （now() > TimerEvent.arrtive_time）
    // 2. arrtive_time 如何让 eventloop 监听

    int timeout = g_epoll_max_timeout; 
    epoll_event result_events[g_epoll_max_events];
    // DEBUGLOG("now begin to epoll_wait");
    int rt = epoll_wait(m_epoll_fd, result_events, g_epoll_max_events, timeout); //每隔10秒返回
    // DEBUGLOG("now end epoll_wait, rt = %d", rt);

    if (rt < 0) {
      ERRORLOG("epoll_wait error, errno=%d, error=%s", errno, strerror(errno));
    } else {
      for (int i = 0; i < rt; ++i) {
        epoll_event trigger_event = result_events[i];
        FdEvent* fd_event = static_cast<FdEvent*>(trigger_event.data.ptr);
        if (fd_event == NULL) {
          ERRORLOG("fd_event = NULL, continue");
          continue;
        }

        // int event = (int)(trigger_event.events); 
        // DEBUGLOG("unkonow event = %d", event);

        if (trigger_event.events & EPOLLIN) { 

          // DEBUGLOG("fd %d trigger EPOLLIN event", fd_event->getFd())
          addTask(fd_event->handler(FdEvent::IN_EVENT));
        }
        if (trigger_event.events & EPOLLOUT) { 
          // DEBUGLOG("fd %d trigger EPOLLOUT event", fd_event->getFd())
          addTask(fd_event->handler(FdEvent::OUT_EVENT));
        }

        // EPOLLHUP EPOLLERR
        if (trigger_event.events & EPOLLERR) {
          DEBUGLOG("fd %d trigger EPOLLERROR event", fd_event->getFd())
          // 删除出错的套接字
          deleteEpollEvent(fd_event);
          if (fd_event->handler(FdEvent::ERROR_EVENT) != nullptr) {
            DEBUGLOG("fd %d add error callback", fd_event->getFd())
            addTask(fd_event->handler(FdEvent::OUT_EVENT));
          }
        }
      }
    }
    
  }

}

void EventLoop::wakeup() {
  INFOLOG("WAKE UP");
  m_wakeup_fd_event->wakeup();
}

void EventLoop::stop() {
  m_stop_flag = true;
  wakeup();
}

void EventLoop::dealWakeup() {

}

void EventLoop::addEpollEvent(FdEvent* event) {
  if (isInLoopThread()) {
    ADD_TO_EPOLL();
  } else {
    auto cb = [this, event]() {
      ADD_TO_EPOLL();
    };
    addTask(cb, true);
  }

}

void EventLoop::deleteEpollEvent(FdEvent* event) {
  if (isInLoopThread()) {
    DELETE_TO_EPOLL();
  } else {

    auto cb = [this, event]() {
      DELETE_TO_EPOLL();
    };
    addTask(cb, true);
  }

}

void EventLoop::addTask(std::function<void()> cb, bool is_wake_up /*=false*/) {
  ScopeMutex<Mutex> lock(m_mutex);
  m_pending_tasks.push(cb);
  lock.unlock();

  if (is_wake_up) {
    wakeup();
  }
}

bool EventLoop::isInLoopThread() {
  return getThreadId() == m_thread_id;
}


EventLoop* EventLoop::GetCurrentEventLoop() {
  if (t_current_eventloop) {
    return t_current_eventloop;
  }
  t_current_eventloop = new EventLoop();
  return t_current_eventloop;
}


bool EventLoop::isLooping() {
  return m_is_looping;
}

}
