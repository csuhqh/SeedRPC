#include <stdio.h>
#include <pthread.h>
#include "threadpool.h"


void* testFun(void* arg){
    printf("tid=%lu, task_number=%d, working\n", pthread_self(), *(int *)arg);
}

int main(){
    /**
     * CircularQueue test code
    */
    hqh::CircularQueue<int>* queue = new hqh::CircularQueue<int>(4);
    std::cout << queue->getSize() << std::endl; //0
    queue->push(45);
    queue->push(5);
    queue->push(56);
    queue->push(78);
    std::cout << queue->getSize() << std::endl; 
    std::cout << queue->front() << std::endl; 
    queue->pop();
    std::cout << queue->front() << std::endl; // 
    std::cout << queue->getSize() << std::endl;
    queue->push(10);
    std::cout << queue->getSize() << std::endl;
    std::cout << queue->front() << std::endl;
    printf("进入main\n");
    /**
     * Thread pool test code
    */
    // hqh::ThreadPool* pool = new hqh::ThreadPool(3, 10, 20);
    // for(int i = 0; i < 500; ++i){
    //     int* num = new int(i);
    //     pool->submit(testFun, num);
    //     printf("任务提交: id=%d\n", i);

    // }
    // printf("任务提交完毕!\n");
    // printf("main call destory\n");
    // pool->destory();
    return 0;
}
