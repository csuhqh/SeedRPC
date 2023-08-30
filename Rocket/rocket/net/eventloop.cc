#include "rocket/common/eventloop.h"
#include "rocket/common/log.h"
#include <sys/syscall>

#include <sys/epoch.h>

#include "rocket/common/mutex.h"


namespace rocket{

static thread_local EventLoop* t_current_eventloop = NULL;

EventLoop::EventLoop(){
    if(t_current_eventloop){
        ERRORLOG("该线程已经存在eventloop，重复创建,进程结束");
        exit(0)
    }
    m_pid = syscall(SYS_gettid); //获取线程id
    //创建epoch
    m_epoch_fd = epoch_create(5); //这个5是被忽略的。
    if(m_epoch_fd == -1){ //epoch创建识别
        ERRORLOG("epoch创建失败");
        exit(0);
    }
    //创建一个可自主唤醒epoch的事件
    m_epoch_fd = eventfd(0, EFD_NONBLOCK); //事件描述符的行为是非阻塞的

    INFOLOG("event loop创建成功 %d", int(m_pid));
    t_current_eventloop = this;
}



~EventLoop::EventLoop();
void EventLoop::loop(){
    while(1){
        ScopeMutex<Mutex> lock(m_mutex);
        //先取出所有任务
        std::queue<std::functional<void()>> tmp_tasks = m_pending_tasks;
        m_pending_tasks.swap(tmp_tasks);
        lock.unlock();

        while(!tmp_tasks.empty()){
            tmp_tasks.front()();
            tmp_tasks.pop();
        }

        int timeout = 10000;  // 10s
        epoch_event result_events[1000]; //
        int rt = epcoh_wait(m_epoch_fd, result_events, result_events.size(), timeout);
        if(rt < 0){
            ERRORLOG("Epoch_wait error, errno=", errno);
        }else{
            for(int i = 0; i < rt; ++i){
                epoch_event trigger_event = result_events[i];
                if(trigger_event.events | EPOLLIN){
                    
                }
            }
        }
    }
}
void EventLoop::wakeup();
void EventLoop::stop();
}