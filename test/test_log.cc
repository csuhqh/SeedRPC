#include "src/common/log.h"
#include <iostream>
#include <pthread.h>

#include <unistd.h>
void* fun(void*){
    DEBUGLOG("i am thread");
    return nullptr;
}

int main(){
    pthread_t thread;
    pthread_create(&thread, nullptr, fun, nullptr);

    DEBUGLOG("i am main %s", "hqh ");
    DEBUGLOG("i am main two ");
    pthread_join(thread, nullptr);
    return 0;
}