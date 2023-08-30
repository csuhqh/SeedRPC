cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 11) # 设置C++标准版本
set(CMAKE_CXX_STANDARD_REQUIRED ON) # 强制使用指定的C++标准版本

project(hqh_rocket)
set(PROJ_PATH /home/hqh/Desktop/C++Proj/Rocket)
include_directories(${PROJ_PATH})

include_directories(${PROJ_PATH}/mylibs/jsoncpp-1.9.2/include)  # 包含头文件所在目录
link_directories(${PROJ_PATH}/mylibs/jsoncpp-1.9.2/build/src/lib_json) # 库文件所在目录 （在build文件夹下）

# add_executable(main reocket/common/main.cpp)
add_executable(test_log test/test_log.cc rocket/common/log.cc)
add_executable(test_json test/test_config.cc)

target_link_libraries(test_log pthread) #需要添加静态链接库 libpthread.a
target_link_libraries(test_json jsoncpp) 