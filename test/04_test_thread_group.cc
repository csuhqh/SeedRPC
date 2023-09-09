#include "src/reactor/io_thread_group.h"
#include <memory>


int main(){
  seedrpc::IOThreadGroup::s_ptr p = std::make_shared<seedrpc::IOThreadGroup>(2);
  p->start();
  p->join();  //一定要join，不然由于主程序结束，产生子线程的epoll错误
  return 0;
}