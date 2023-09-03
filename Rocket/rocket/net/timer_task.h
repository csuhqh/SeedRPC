#ifndef ROCKET_NET_TIMER_TASK_H
#define ROCKET_NET_TIMER_TASK_H
#include <functional>
#include <memory>
#include "rocket/common/util.h"
namespace rocket{
class TimerTask{
    public:
    typedef std::shared_ptr<TimerTask> s_ptr; //定义一个只能指针
    //这里的callback不是eventloop调用它，而是eventloop调用Timer的OnTimer间接调用
    TimerTask(time_t delay, bool repead, std::function<void()> taskFun): m_delay(delay), m_isRepeat{repead}, m_taskFun(taskFun){
        m_expire = getTick() + delay;
    }; 
    void setCancel(bool isCancel){m_isCancel = isCancel;}
    bool isCancel(){return m_isCancel;}
    bool isRepeat() {return m_isRepeat;};
    time_t get_delay(){return m_delay;};
    time_t get_expire(){return m_expire;};
    void setExpire(time_t expire){m_expire = expire;}

    std::function<void()> getTaskFunc(){
        return m_taskFun;
    }

    private:
        time_t m_delay;  // ms
        time_t m_expire; // ms
        bool m_isCancel{false}; //是否取消该任务
        bool m_isRepeat{false}; //是否重复执行
        std::function<void()> m_taskFun;
};
}

#endif