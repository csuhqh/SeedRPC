#ifndef ROCKET_COMMON_LOG_H
#define ROCKET_COMMON_LOG_H


#include <string>
#include <memory>
#include <vector>
#include <queue>
#include "mutex.h"

namespace rocket {

template<typename... Args> //可变参数模版
std::string formatString(const char* str, Args&&... args) {

  int size = snprintf(nullptr, 0, str, args...); //计算格式化后，字符串的大小

  std::string result;
  if (size > 0) {
    result.resize(size);
    snprintf(&result[0], size + 1, str, args...);
  }
  if(result[result.size() - 1] == '\n') return result;
  result += "\n";
  return result;
}

    //__VA_ARGS__表示可变参数
#define DEBUGLOG(str, ...) \
    std::string msg = ((new rocket::LogEvent(rocket::LogLevel::DEBUG))->toString());\
    msg += rocket::formatString(str, ##__VA_ARGS__); \
    (rocket::Logger::getGlobalLogger())->pushLog(msg);\
    (rocket::Logger::getGlobalLogger())->log();

#define INFOLOG(str, ...) std::string msg = ((new rocket::LogEvent(rocket::LogLevel::INFO))->toString());\
    msg += rocket::formatString(str, ##__VA_ARGS__); \
    (rocket::Logger::getGlobalLogger())->pushLog(msg);\
    (rocket::Logger::getGlobalLogger())->log();
#define ERRORLOG(str, ...) std::string msg = ((new rocket::LogEvent(rocket::LogLevel::ERROR))->toString());\
    msg += rocket::formatString(str, ##__VA_ARGS__); \
    (rocket::Logger::getGlobalLogger())->pushLog(msg);\
    (rocket::Logger::getGlobalLogger())->log();
#define WARNLOG(str, ...) std::string msg = ((new rocket::LogEvent(rocket::LogLevel::WARN))->toString());\
    msg += rocket::formatString(str, ##__VA_ARGS__); \
    (rocket::Logger::getGlobalLogger())->pushLog(msg);\
    (rocket::Logger::getGlobalLogger())->log();



enum LogLevel {
    INFO = 1,
    DEBUG = 2,
    WARN = 3,
    ERROR = 4
};

class LogEvent{
    public:
        LogEvent(LogLevel level);
        std::string getFileName() const{
            return m_file_name;
        }
        LogLevel getLogLevel() const{
            // m_level = 3;  //会报错吗
            return m_level;
        }
        void printLog();
        std::string toString();
    private:
        //日志的时间
        std::string m_file_name; //文件名
        int32_t m_file_line; // 行号
        int32_t m_pid; //进程号
        int32_t m_thread_id; //线程号

        LogLevel m_level; //日志级别

};




class Logger{
    private:
    // typedef static std::shared_ptr<Logger> s_ptr = new Logger(); //指针私有化
        Mutex m_mutex; //直接定义一个锁对象
        static Logger* ptr;
        Logger(){}; //构造函数私有化
        Logger(Logger const&) = delete; //防止拷贝
        std::queue<std::string> m_buffer;
        LogLevel m_level;
    public:
        void log();
        void pushLog(const std::string& msg);
        static Logger* getGlobalLogger() {return ptr;}; //单例模式 [作者使用单例模式错误]

};

}
#endif
