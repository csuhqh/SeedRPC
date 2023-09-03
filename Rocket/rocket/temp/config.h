#ifndef ROCKET_COMMON_CONFIG_H
#define ROCKET_COMMON_CONFIG_H

#include <map>


namespace rocket{



class Config{
    public:
        Config(const char* jsonfile);
    public:
        Config* getGlobalConfig();
    public:
        std::string m_log_level;
        std::string m_log_filename;
        std::string m_log_filepath;
        int m_log_max_filesize{0};
        int m_log_sync_inteval{0}; //日志间隔ms
        int m_port{0};
        int m_io_threads{0};
        // Json::Value* configfile{nullptr};
        

};

}







#endif