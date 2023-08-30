#ifndef ROCKET_NET_EVENTLOOP_H
#def ROCKET_NET_EVENTLOOP_H
namespace rocket{
#include <pthread.h>
#include <functional>
#include "rocket/common/mutex.h"

class EventLoop{ //每个线程只能有一个

    public:
        EventLoop();
        ~EventLoop();
        void loop();
        


    private:
        Mutex m_mutex;
        pid_t m_pid; //一个线程对应一个eventloop
        epoch_event m_wakeup_fd;
        std::set<int> m_listen_fds; //监听的所有套接字
        //所有待执行的任务队列
        std::queue<std::functional<void()>> m_pending_tasks; // C++11 引入的函数对象的包装器

}


}



#endif