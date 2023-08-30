#ifndef HQH_THREADPOOL_H
#define HQH_THREADPOOL_H

#include <stdio.h>
#include <pthread.h>
#include <iostream>
#include <stdexcept>
#include <unistd.h>

namespace hqh{

#define ADD_THREAD_NUM 2
#define KILL_THREAD_NUM 2;

//任务结构体
typedef struct Task{
    void* (*function)(void* arg);  //定义函数指针 返回值 (*p) (参数类型)
    void* arg;
    Task(){function=nullptr, arg=nullptr;};
    Task(void* (*function) (void*), void* arg): function(function), arg(arg){};
}Task;
//循环队列(非安全)
template <typename T>
class CircularQueue{
    public:
        CircularQueue(int capacity): m_capacity(capacity){
            elements = new T[capacity + 1]; //需要留出一个空位
        }
        ~CircularQueue(){
            delete[] elements;
        }
        bool push(const T& item){
            if(getSize() == m_capacity){
                return false;
            }
            elements[m_rear] = item;
            m_rear += 1;
            m_size += 1;
            return true;
        }
        bool pop(){
            if(isEmpyt()) return false;
            m_front += 1;
            m_size -= 1;
            return true;
        }
        //取头元素
        const T& front() const{
            if(isEmpyt()) throw std::runtime_error("CircularQueue is empty! pop() fail");
            return elements[m_front];
        }
        bool isEmpyt() const{
            return m_size == 0;
        }
        int getSize() const{
            return m_size; // (rear - front + maxSize) % maxSize
        }
    private:
        T* elements;
        int m_size{0}, m_front{0}, m_rear{0};
        int m_capacity;
};

//线程池
static void* worker(void*);  //实现让pthread_create 调用类内函数 [桥梁]
static void* manager(void*);
class ThreadPool{
    public:
        ThreadPool(int min, int max, int capacity);
        ~ThreadPool();
        void killThread();
        void submit(void* (*func)(void*), void* arg );

        int getBusyNum();
        int getLiveNum();

        int destory();
        void* workerImp();
        void* managerImp();


    private:
        CircularQueue<Task>* m_taskQueue; //任务队列(环形队列)
        pthread_t* m_threadIDs;
        pthread_t m_managerID;

        int m_minNum, m_maxNum; //最小线程数，最大线程数量
        int m_capacity; //任务队列的容量
        int m_busyNum{0}, m_liveNum, m_exitNum{0}; //定义忙线程，存活线程，被销毁的线程 （当前线程池的状态）   
        int m_shutdown{0}; //是不是要销毁线程池， 1为销毁

        pthread_mutex_t m_mutexQueue; //锁，对队列的锁
        pthread_mutex_t m_mutexBusyNum; //锁，对忙线程进行添加


        pthread_cond_t m_condNotEmpty; //条件变量，队列是不是空了
        pthread_cond_t m_condNotFull;  //条件变量，表示信号队列没满
        
    
};

static void* worker(void* threadPool){
    ThreadPool *ptr = (ThreadPool* )threadPool;
    ptr->workerImp();
}
static void* manager(void* threadPool){
    ThreadPool *ptr = (ThreadPool*)threadPool;
    ptr->managerImp();
}


}


#endif