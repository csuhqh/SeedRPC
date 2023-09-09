#include "src/reactor/eventloop.h"

#include <string.h>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <sys/socket.h>

#include <memory>

#include "src/common/log.h"
#include "src/common/util.h"
#include "src/reactor/timer_fd_event.h"

namespace seedrpc {

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

    initWakeUpFdEevent();  // 添加唤醒 [监控一个eventfd的写事件，如果要主动唤醒epoll，对它进行写入]
    initTimer();           // 添加定时器 [添加一个timer的文件描述符]

    INFOLOG("succ create event loop in thread %d", m_thread_id);
    t_current_eventloop = this;
}

void EventLoop::initTimer() {
    m_timer_fd_event = std::make_shared<TimerFdEvent>();
    addEpollEvent(m_timer_fd_event);
}

void EventLoop::initWakeUpFdEevent() {
    m_wakeup_fd_event = std::make_shared<WakeUpFdEvent>();
    addEpollEvent(m_wakeup_fd_event);
}

void EventLoop::addTimerTask(TimerTask::s_ptr p) {
    m_timer_fd_event->addTimerTask(std::move(p));
}

void EventLoop::loop() {
    m_is_looping = true;
    while (!m_stop_flag) {
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

        epoll_event result_events[g_epoll_max_events];
        // DEBUGLOG("now begin to epoll_wait");
        int rt = epoll_wait(m_epoll_fd, result_events, g_epoll_max_events, -1);
        if (rt < 0) {
            ERRORLOG("epoll_wait error, errno=%d, error=%s", errno, strerror(errno));
        } else {
            for (int i = 0; i < rt; ++i) {
                epoll_event trigger_event = result_events[i];
                // FdEvent::s_ptr fd_event((FdEvent*)(m_timer_fd_event.get())); //裸指针给智能指针（危险！！）【崩溃】
                // FdEvent* fd_event = static_cast<FdEvent*>(trigger_event.data.ptr);
                FdEvent::s_ptr fd_event(m_listened_event[trigger_event.data.u32]);
                if (fd_event == NULL) {
                    ERRORLOG("fd_event = NULL, continue");
                    continue;
                }

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

//?
void EventLoop::addEpollEvent(FdEvent::s_ptr event) {
    if (isInLoopThread()) {
        registerFdEvent(event);
    } else {
        auto cb = [this, event]() {
            registerFdEvent(event);
        };
        addTask(cb, true);
    }
}

void EventLoop::deleteEpollEvent(FdEvent::s_ptr event) {
    if (isInLoopThread()) {
        unregisterFdEvent(event);
    } else {
        auto cb = [this, event]() {
            unregisterFdEvent(event);
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

bool EventLoop::registerFdEvent(FdEvent::s_ptr event) {
    int id = event->getId();
    auto it = m_listened_event.find(id);
    int op = it != m_listened_event.end() ? EPOLL_CTL_MOD : EPOLL_CTL_ADD;
    m_listened_event.insert(std::make_pair(id, event));
    epoll_event tmp = event->getEpollEvent();
    //
    int rt = epoll_ctl(m_epoll_fd, op, event->getFd(), &tmp);
    if (rt == -1) {
        ERRORLOG("failed epoll_ctl when add fd, errno=%d, error=%s", errno, strerror(errno));
        return false;
    }
    return true;
}

bool EventLoop::unregisterFdEvent(FdEvent::s_ptr event) {
    int id = event->getId();
    auto it = m_listened_event.find(id);
    if (it != m_listened_event.end()) {  // 可以删除
        m_listened_event.erase(it);
        epoll_event tmp = event->getEpollEvent();
        int rt = epoll_ctl(m_epoll_fd, EPOLL_CTL_DEL, event->getFd(), NULL);
        if (rt == -1) {
            ERRORLOG("failed epoll_ctl when add fd, errno=%d, error=%s", errno, strerror(errno));
            return false;
        }
        DEBUGLOG("delete event success, fd[%d]", event->getFd());
        return true;
    } else {
        ERRORLOG("unregister a not exist Fd Event, id=%d", id);
        return false;
    }
}

bool EventLoop::isInLoopThread() {
    return getThreadId() == m_thread_id;
}

bool EventLoop::isLooping() {
    return m_is_looping;
}

}  // namespace seedrpc
