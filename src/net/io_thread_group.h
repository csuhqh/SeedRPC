#ifndef SRC_NET_IO_THREAD_GROUP_H
#define SRC_NET_IO_THREAD_GROUP_H

#include <memory>
#include "src/net/io_thread.h"

namespace seedrpc{
class IOThreadGroup{


public:
  typedef std::shared_ptr<IOThreadGroup> s_ptr;
  IOThreadGroup(int size); //池中存放多少线程eventloop
  void start();
  void join();
  IOThread::s_ptr getIOThread(); //从池子中获得一个IO线程


private:
  int m_size{0};
  std::vector<IOThread::s_ptr> m_io_threads;
  int m_index{0};



};
}


#endif