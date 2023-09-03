#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <memory>
#include "rocket/common/log.h"
// #include "rocket/common/config.h"
#include "rocket/net/fd_event.h"
#include "rocket/net/eventloop.h"
#include "rocket/net/timer_fd_event.h"
#include "rocket/net/timer_task.h"

void* myfun(){
    
}

int main(){
    rocket::EventLoop* eventloop = new rocket::EventLoop();
    printf("test timer\n");
    int i = 0;
    rocket::TimerTask::s_ptr task = std::make_shared<rocket::TimerTask>(2000, true, [&i](){
        printf("i am test_timer, print 1 times per second, current i = %d\n", i++);
    });
    eventloop->addTimerTask(task);
    eventloop->loop();
    return 0;

}