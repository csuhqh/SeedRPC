#ifndef ROCKET_NET_TIMER_FD_EVENT_H
#define ROCKET_NET_TIMER_FD_EVENT_H

#include <string.h>
#include <sys/time.h>
#include <sys/timerfd.h>
#include <unistd.h>

#include <chrono>
#include <map>

#include "rocket/common/log.h"
#include "rocket/common/mutex.h"
#include "rocket/net/fd_event.h"
#include "rocket/net/timer_task.h"

namespace rocket {

class TimerFdEvent : public FdEvent {  // 父类FdEvent，意味着它要被注册到epoll中
public:
    TimerFdEvent() {
        m_fd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
        listen(FdEvent::IN_EVENT,
               std::bind(&TimerFdEvent::onTimer, this));  // 需要这样绑定
    };

    ~TimerFdEvent(){};
    // 添加定时任务
    std::pair<time_t, uint64_t> addTimerTask(TimerTask::s_ptr timeTask) {
        bool reset_timefd = false;  // 是否需要重新设置timerfd的触发时间
        ScopeMutex<Mutex> lock(m_mutex);
        if (m_pending_events.empty())
            reset_timefd = true;
        else {
            auto it = m_pending_events.begin();
            if ((*it).second->get_expire() >
                timeTask
                    ->get_expire()) {  // 比较新添加的时间任务与最新的任务，谁先执行
                reset_timefd = true;
            }
        }
        if (reset_timefd) {
            timespec ts;
            memset(&ts, 0, sizeof(ts));
            time_t delay = timeTask->get_delay();
            ts.tv_sec = delay / 1000;
            ts.tv_nsec = (delay % 1000) / 1000000;
            itimerspec value;
            memset(&value, 0, sizeof(value));
            value.it_value = ts;
            timerfd_settime(m_fd, 0, &value, NULL);  // 0表示相对时间
        };
        m_pending_events.emplace(timeTask->get_expire(), std::move(timeTask));
        lock.unlock();
    }
    // 删除定时任务
    void deleteTimerEvent(TimerTask::s_ptr timeTask) {
        timeTask->setCancel(true);
        ScopeMutex<Mutex> lock(m_mutex);
        auto begin = m_pending_events.lower_bound(timeTask->get_expire());
        auto end = m_pending_events.upper_bound(timeTask->get_expire());
        auto it = begin;
        for (it = begin; it != end; ++it) {
            if (it->second == timeTask)
                break;
        }
        if (it != end)
            m_pending_events.erase(it);
        lock.unlock();
        DEBUGLOG("成功删除定时任务: %lld", timeTask->get_expire());
    }
    // 执行定时任务：由epoll回调onTimer，onTimer再执行定时任务
    void onTimer() {  // 当发送了 IO 事件后，eventloop 会执行这个回调函数
        // 读空缓冲区
        char buf[8];
        while (true) {
            if ((read(m_fd, buf, 8) == -1) && errno == EAGAIN) {
                break;
            }
        }
        // 取出所有待执行的任务
        std::vector<TimerTask::s_ptr> tmps;
        std::vector<TimerTask::s_ptr> tasks;

        time_t now = getTick();  // 获取当前时间
        ScopeMutex<Mutex> lock(m_mutex);
        auto it = m_pending_events.begin();
        for (; it != m_pending_events.end(); ++it) {
            if ((*it).first <= now) {  // 符合时间的，待执行任务
                if (!(*it).second->isCancel()) {
                    tmps.push_back((*it).second);
                }
            } else
                break;
        }
        m_pending_events.erase(m_pending_events.begin(), it);
        lock.unlock();
        // 重复的需要加入
        for (auto i = tmps.begin(); i != tmps.end(); ++i) {
            (*i)->getTaskFunc()();  // 调用
            if ((*i)->isRepeat()) {
                (*i)->setExpire(now + (*i)->get_delay());
                addTimerTask((*i));
            }
        }
    }

private:
    std::multimap<time_t, TimerTask::s_ptr> m_pending_events;
    Mutex m_mutex;
};

}  // namespace rocket
#endif