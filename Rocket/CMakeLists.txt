cmake_minimum_required(VERSION 3.0)

project(hqhRocket)
set(PROJ_PATH ${CMAKE_SOURCE_DIR})
include_directories(${PROJ_PATH})
message(${PROJ_PATH})
# include_directories(${PROJ_PATH}/rocket/common)
# include_directories(${PROJ_PATH}/rocket/net)


include_directories(${PROJ_PATH}/mylibs/jsoncpp-1.9.2/include)  # 包含头文件所在目录
link_directories(${PROJ_PATH}/mylibs/jsoncpp-1.9.2/build/src/lib_json) # 库文件所在目录 （在build文件夹下）

# # file(ALL_SOUR "${PROJ_PATH}/reocket/common/*.cc" "${PROJ_PATH}/rocket/net/*.cc")
aux_source_directory(${PROJ_PATH}/rocket/common COMMON_SRCS)
aux_source_directory(${PROJ_PATH}/rocket/net NET_SRCS)


add_executable(test_log test/test_log.cc rocket/common/log.cc)
# add_executable(test_json test/test_config.cc)
add_executable(test_eventloop test/test_eventloop ${NET_SRCS} ${COMMON_SRCS})
add_executable(test_timer test/test_timer.cc ${NET_SRCS} ${COMMON_SRCS})


target_link_libraries(test_log pthread) #需要添加静态链接库 libpthread.a
# target_link_libraries(test_json jsoncpp) 
target_link_libraries(test_eventloop pthread)
target_link_libraries(test_timer pthread)

# include_directories(${PROJ_PATH}/mylibs/jsoncpp-1.9.2/include)  # 包含头文件所在目录
# link_directories(${PROJ_PATH}/mylibs/jsoncpp-1.9.2/build/src/lib_json) # 库文件所在目录 （在build文件夹下）

# # add_executable(main reocket/common/main.cpp)
# add_executable(test_log test/test_log.cc rocket/common/log.cc)
# add_executable(test_json test/test_config.cc)

# target_link_libraries(test_log pthread) #需要添加静态链接库 libpthread.a
# target_link_libraries(test_json jsoncpp) 