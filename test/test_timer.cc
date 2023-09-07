#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <memory>
#include "src/common/log.h"
// #include "src/common/config.h"
#include "src/net/fd_event.h"
#include "src/net/eventloop.h"
#include "src/net/timer_fd_event.h"
#include "src/net/timer_task.h"

void* myfun(){
    
}

int main(){
    seedrpc::EventLoop* eventloop = new seedrpc::EventLoop();
    printf("test timer\n");
    int i = 0;
    seedrpc::TimerTask::s_ptr task = std::make_shared<seedrpc::TimerTask>(2000, true, [&i](){
        printf("i am test_timer, print 1 times per second, current i = %d\n", i++);
    });
    eventloop->addTimerTask(task);
    eventloop->loop();
    return 0;

}