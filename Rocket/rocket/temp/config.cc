#include "rocket/common/config.h"
#include <json/json.h>
#include <fstream>
#include <iostream>
namespace rocket{


Config::Config(const char* jsonfile){
    //读取解析json文件
    //读取json文件
    std::ifstream file(jsonfile);
    if(!file.is_open()){
        std::cout << "加载配置文件失败!" << std::endl;
        return;
    }
    Json::Value root;
    file >> root;
    file.close();
    //解析json
    // m_log_level = root["log"]["log_level"];
    // m_port = root["server"]["port"];
    // m_io_threads = root["server"]["io_threads"];


}


}

