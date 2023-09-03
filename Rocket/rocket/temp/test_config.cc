#include <iostream>
#include <fstream>
#include <json/json.h>

int main() {
    // 打开 JSON 文件
    std::ifstream file("/home/hqh/Desktop/C++Proj/Rocket/rocket/config.json");
    if (!file.is_open()) {
        std::cerr << "Failed to open file." << std::endl;
        return 1;
    }

    // 从文件中读取 JSON 数据
    Json::Value root;
    file >> root;
    file.close();

    // 输出 JSON 数据到控制台
    std::cout << "Name: " << root["name"].asString() << std::endl;
    std::cout << "Age: " << root["age"].asInt() << std::endl;
    std::cout << "City: " << root["city"].asString() << std::endl;

    return 0;
}