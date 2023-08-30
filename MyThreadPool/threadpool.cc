#include "threadpool.h"
#include <stdio.h>
#include <pthread.h>
#include <iostream>
#include <stdexcept>
#include <unistd.h>


namespace hqh{
ThreadPool::ThreadPool(int min, int max, int capacity): m_minNum(min), m_maxNum(max), m_capacity(capacity){
        m_threadIDs = new pthread_t[max];
        if(m_threadIDs == nullptr){
            printf("new pthread_t fail ...\n");
        }
        m_taskQueue = new CircularQueue<Task>(capacity);
        if(m_taskQueue == nullptr){
            printf("new CircularQueue fail... \n");
            exit(0);
        }

        pthread_mutex_init(&m_mutexQueue, nullptr);
        pthread_mutex_init(&m_mutexBusyNum, nullptr);
        pthread_cond_init(&m_condNotEmpty, nullptr);
        pthread_cond_init(&m_condNotFull, nullptr);

        // 创建工作线程（不断从任务队列中取出任务）
        m_liveNum = min;
        for(int i = 0; i < min; ++i){
            pthread_create(&m_threadIDs[i], nullptr, worker, this);
        }
        // 创建manager线程
        pthread_create(&m_managerID, nullptr, manager, this);
}

void ThreadPool::killThread(){ //线程退出
    pthread_t tid = pthread_self();
    for(int i = 0; i < m_maxNum; ++i){
        if(m_threadIDs[i] == tid){
            m_threadIDs[i] = 0;
            break;
        }
    }
    printf("threadExit() called, %lu exiting\n", tid);
    pthread_exit(nullptr);
}

void ThreadPool::submit(void* (*func)(void*), void* arg ){
    pthread_mutex_lock(&m_mutexQueue); //死锁。。
    while(m_taskQueue->getSize() == m_capacity && !m_shutdown){ //消息队列满了/线程池关闭
        printf("任务队列已满!等待消费者消费.\n");
        //阻塞生产者线程
        pthread_cond_wait(&m_condNotFull, &m_mutexQueue);
    }
    if(m_shutdown){
        pthread_mutex_unlock(&m_mutexQueue);
        return;
    }
    //添加任务
    Task task;  
    task.function = func;
    task.arg = arg;
    m_taskQueue->push(task);
    //唤醒
    pthread_cond_signal(&m_condNotEmpty); //唤醒工作线程
    pthread_mutex_unlock(&m_mutexQueue);
}

int ThreadPool::getBusyNum(){
    pthread_mutex_lock(&m_mutexBusyNum);
    int busyNum = m_busyNum;
    pthread_mutex_unlock(&m_mutexBusyNum);
    return busyNum;
}

int ThreadPool::getLiveNum(){
    pthread_mutex_lock(&m_mutexBusyNum);
    int liveNum = m_liveNum;
    pthread_mutex_unlock(&m_mutexBusyNum);
    return liveNum;
}

int ThreadPool::destory(){
    //关闭线程池
    m_shutdown = 1;
    printf("manager join ....\n");
    //阻塞回收管理者线程
    pthread_join(m_managerID, nullptr);
    //唤醒阻塞的消费者线程
        printf("存活的线程个数 %d\n", m_liveNum);
    for(int i = 0; i < m_liveNum; ++i){
        pthread_cond_broadcast(&m_condNotEmpty);
    }
    printf("workers join ...\n");
    // 等待所有worker线程结束
    for(int i = 0; i < m_maxNum; ++i){
        if(m_threadIDs[i] != 0){
            pthread_join(m_threadIDs[i], nullptr);
        }
    }
    printf("join end.");
    for(int i = 0; i < m_maxNum; ++i){
        printf("destory %lu\n", m_threadIDs[i]);
    }
    pthread_mutex_destroy(&m_mutexBusyNum);
    pthread_mutex_destroy(&m_mutexQueue);
    pthread_cond_destroy(&m_condNotEmpty);
    pthread_cond_destroy(&m_condNotFull);
    //释放堆内存
    delete m_taskQueue;
    delete m_threadIDs;
    return 0;
}

void* ThreadPool::workerImp(){ //子线程任务函数
    while(true){
        pthread_mutex_lock(&m_mutexQueue); //对该队列加锁
        while(m_taskQueue->isEmpyt() && !m_shutdown){ //任务队列没有有任务，并且线程池存活
            printf("阻塞worker线程: %lu\n", pthread_self());
            //柱塞工作线程
            pthread_cond_wait(&m_condNotEmpty, &m_mutexQueue);
            //让当前工作的线程自杀
            if(m_exitNum > 0){
                m_exitNum--;
                if(m_liveNum > m_minNum){
                    m_liveNum--;
                    pthread_mutex_unlock(&m_mutexQueue);
                    killThread();
                }
            }
        }
        //判断线程池是否被关闭了
        if(m_shutdown){
            pthread_mutex_unlock(&m_mutexQueue);
            killThread(); //当前线程退出
        }
        // 从任务队列中取出一个任务
        Task task = m_taskQueue->front();
        m_taskQueue->pop();
        pthread_cond_signal(&m_condNotFull); //通知生产者
        pthread_mutex_unlock(&m_mutexQueue); //对该队列解锁
        //执行任务
        pthread_mutex_lock(&m_mutexBusyNum);
        ++m_busyNum;
        pthread_mutex_unlock(&m_mutexBusyNum);

        task.function(task.arg); // 等价写法 (*task.function)(task.arg)
        // delete task.arg; //task.arg参数可能在堆区
        // task.arg = nullptr;\

        printf("thread %ld end working.. \n", pthread_self());
        pthread_mutex_lock(&m_mutexBusyNum);
        --m_busyNum;
        pthread_mutex_unlock(&m_mutexBusyNum);
    }
}


void* ThreadPool::managerImp(){
    while(!m_shutdown){
        sleep(1);   
        //获取线程池中任务的数量和当前线程的数量
        pthread_mutex_lock(&m_mutexQueue);
        int tasknum = m_taskQueue->getSize();
        int liveNum = m_liveNum;
        pthread_mutex_unlock(&m_mutexQueue);
        // printf("获取线程池中任务的数量和当前线程的数量\n");
        //去除忙的线程的数量
        pthread_mutex_lock(&m_mutexBusyNum);
        int busyNum = m_busyNum;
        pthread_mutex_unlock(&m_mutexBusyNum);
        // printf("去除忙的线程的数量\n");
        // 添加线程 (存活的线程数量 < 任务数量 && 存活的线程数 < 最大线程数)
        if(tasknum > liveNum && liveNum < m_maxNum){ // 线程干活不过来
            // printf("添加2个线程\n");
            pthread_mutex_lock(&m_mutexQueue);
            int count = 0;
            for(int i = 0; i < m_maxNum && count < ADD_THREAD_NUM; ++i){
                if(m_threadIDs[i] == 0){
                    printf("成功创建一个线程++++++++++++++\n");
                    pthread_create(&m_threadIDs[i], nullptr, worker, this);
                    count++;
                    m_liveNum++; //添加线程
                }
            }
            pthread_mutex_unlock(&m_mutexQueue);
            // 销毁线程 （存活的线程 > 忙的线程 * 2） && 存活的线程 > 最小线程数量
        }else if(busyNum * 2 < liveNum && liveNum > m_minNum){
            // printf("销毁2线程\n"); 
            pthread_mutex_lock(&m_mutexQueue);
            m_exitNum = KILL_THREAD_NUM;
            pthread_mutex_unlock(&m_mutexQueue);
            //让存活的自杀。
            for(int i = 0; i < 2; ++i){
                pthread_cond_signal(&m_condNotEmpty);
            }
        }  
    }
}

}
