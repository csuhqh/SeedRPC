#ifndef SRC_REACTOR_TO_THREAD_H
#define SRC_REACTOR_IO_THREAD_H

#include "src/reactor/eventloop.h"
#include <pthread.h>
#include <semaphore.h>
#include <memory>
/*
一个IOThread就是一个EventLoop
*/
namespace seedrpc {
class IOThread {
  public:
    typedef std::shared_ptr<IOThread> s_ptr;
    IOThread();
    EventLoop::s_ptr getEventLoop();
    void start();
    void join();
    static void* Main(void* arg);
  private:
    pid_t m_thread_id; //线程号
    pthread_t m_thread; //线程句柄
    EventLoop::s_ptr m_event_loop{nullptr};
    sem_t m_init_semaphore; //初始化成功信号量
    sem_t m_start_semaphore; //线程启动信号量
  };

}  // namespace seedrpc

#endif