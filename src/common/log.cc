#include <sys/time.h>
#include "log.h"
// #include <stringstream>
#include <sstream>
#include <iostream>
#include <unistd.h> //使用getpid();
#include <sys/syscall.h> //使用系统调用
// #include <sstream>
// #include <stdio.h>
// #include <assert.h>
// #include <signal.h>



namespace seedrpc{

// static Logger* g_logger = nullptr;

// Logger* Logger::getGlobalLogger(){ //我认为线程不安全
//     if(g_logger) return g_logger;
//     g_logger = new Logger();
//     return g_logger;
// }
Logger* Logger::ptr = new Logger(); //类外初始化
LogEvent::LogEvent(LogLevel level){
    m_level = level;
}


std::string LogLevelToString(LogLevel level){
    switch(level){
        case INFO:
            return "INFO";
            break;
        case DEBUG:
            return "DEUBG";
            break;
        case WARN:
            return "WARN";
            break;
        case ERROR:
            return "ERROR";
            break;
    }
}



std::string LogEvent::toString(){
    timeval now_time; //获取当前时间
    gettimeofday(&now_time, nullptr);
    struct tm now_time_t;
    localtime_r(&(now_time.tv_sec), &now_time_t);

    char buf[128];
    strftime(&buf[0], 128, "%y-%m-%d %H:%M:%S", &now_time_t); //格式化时间
    std::string time_str(buf);
    int ms = now_time.tv_usec / 1000; //毫秒数
    time_str += "." + std::to_string(ms); //打印时间

    m_pid = getpid(); //getPid();
    m_thread_id = syscall(SYS_gettid); //getThreadId();

    std::stringstream ss;
    ss << "[" << LogLevelToString(m_level) << "]" <<"[" << m_pid << ":" << m_thread_id << "]" << "[" << time_str << "]";
    return ss.str();
}

void Logger::pushLog(const std::string& msg){ //一个队列
    ScopeMutex<Mutex> lock(m_mutex); //对{}内上锁
    m_buffer.push(msg);
    lock.unlock(); //可写可不写
}

void Logger::log(){
    ScopeMutex<Mutex> lock(m_mutex);
    std::queue<std::string> tmp = m_buffer;
    m_buffer.swap(tmp); //把buff取出来放到一个临时的队列中
    lock.unlock();
    while(!m_buffer.empty()){ ////多个线程从打印队列中去数据
        std::string msg = m_buffer.front(); 
        m_buffer.pop();
        printf("%s", msg.c_str());
    }
}

}