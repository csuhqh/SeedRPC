#include "src/reactor/io_thread_group.h"
#include <memory>
namespace seedrpc {
IOThreadGroup::IOThreadGroup(int size): m_size(size){
  m_io_threads.resize(size);
  for(size_t i = 0; i < size; ++i){
    m_io_threads[i] = std::make_shared<IOThread>();
  }
}
void IOThreadGroup::start(){
  for(int i = 0; i < m_io_threads.size(); ++i){
    m_io_threads[i]->start();
  }
}
void IOThreadGroup::join(){
  for(int i = 0; i < m_io_threads.size(); ++i){
    m_io_threads[i]->join();
  }
}
IOThread::s_ptr IOThreadGroup::getIOThread(){
  if(m_index = m_io_threads.size()){
    m_index = 0;
  }
  return m_io_threads[m_index++];
}

}  // namespace seedrpc