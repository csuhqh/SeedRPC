#include <pthread.h>
#include <memory>
#include "src/reactor/eventloop.h"
#include "src/reactor/io_thread.h"
#include "src/common/log.h"
#include "src/common/util.h"
namespace seedrpc {

IOThread::IOThread(){
  sem_init(&m_init_semaphore, 0, 0); // 初始化信号量
  sem_init(&m_start_semaphore, 0, 0); // 初始化信号量
  pthread_create(&m_thread, nullptr, &IOThread::Main, this); //创建线程(一定要把自己this穿进去，这样才能和创建的线程沟通)
  sem_wait(&m_init_semaphore); //创建成功了eventloop对象后
  DEBUGLOG("IOThread [%d] create_success", m_thread_id);
  
}

EventLoop::s_ptr IOThread::getEventLoop(){
  return m_event_loop;
}


void IOThread::start(){
  DEBUGLOG("Start invoke IOThread %d", m_thread_id);
  sem_post(&m_start_semaphore);
}

void IOThread::join(){
  pthread_join(m_thread, nullptr);
}

void* IOThread::Main(void* arg){
  IOThread::s_ptr self((IOThread*)arg);
  self->m_event_loop = std::make_shared<EventLoop>();
  self->m_thread_id = getThreadId();
  //说明线程以及启动成功（发出信号量）
  sem_post(&self->m_init_semaphore);
  sem_wait(&self->m_start_semaphore); //等待客户端调用start
  DEBUGLOG("IOThread %d start loop", self->m_thread_id);
  self->m_event_loop->loop();
  return nullptr;
}





}  // namespace seedrpc