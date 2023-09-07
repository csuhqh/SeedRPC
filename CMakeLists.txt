cmake_minimum_required(VERSION 3.0)
project(SeedRPC)
set(PROJ_PATH ${CMAKE_SOURCE_DIR})
include_directories(${PROJ_PATH})
message(${PROJ_PATH})

# 忽略字符串格式化警告
add_compile_options(-Wno-format-security)

include_directories(${PROJ_PATH}/mylibs/jsoncpp-1.9.2/include)  # 包含头文件所在目录
include_directories(${PROJ_PATH}/mylibs/protobuf-3.16.3/src)
link_directories(${PROJ_PATH}/mylibs/jsoncpp-1.9.2/build/src/lib_json) # 库文件所在目录 （在build文件夹下）
link_directories(${PROJ_PATH}/mylibs/protobuf-3.16.3/cmake/build)


set(ALL_SRCS "")
aux_source_directory(${PROJ_PATH}/src/common COMMON_SRCS)
aux_source_directory(${PROJ_PATH}/src/net NET_SRCS)
aux_source_directory(${PROJ_PATH}/src/net/tcp NET_TCP_SRCS)
aux_source_directory(${PROJ_PATH}/src/net/rpc NET_RPC_SRCS)
aux_source_directory(${PROJ_PATH}/src/net/rpc/coder NET_RPC_CODER_SRCS)
list(APPEND ALL_SRCS ${COMMON_SRCS} ${NET_SRCS} ${NET_TCP_SRCS} ${NET_RPC_SRCS} ${NET_RPC_CODER_SRCS})
aux_source_directory(${PROJ_PATH}/src/client CLIENT_SRCS)
aux_source_directory(${PROJ_PATH}/src/server SERVER_SRCS)
aux_source_directory(${PROJ_PATH}/src/pbs PROBU_SRCS)
list(APPEND ALL_SRCS ${CLIENT_SRCS} ${SERVER_SRCS} ${PROBU_SRCS})




add_executable(test_log test/test_log.cc ${ALL_SRCS})
add_executable(test_eventloop test/test_eventloop ${ALL_SRCS})
add_executable(test_timer test/test_timer.cc ${ALL_SRCS})
add_executable(test_thread_group test/test_thread_group.cc ${ALL_SRCS} )
add_executable(test_tcp_connection test/test_tcp_connection.cc ${ALL_SRCS} )
add_executable(test_rpc_server test/test_rpc_server.cc ${ALL_SRCS})
add_executable(test_rpc_client test/test_rpc_client.cc ${ALL_SRCS})


target_link_libraries(test_log pthread protobuf) #需要添加静态链接库 libpthread.a
target_link_libraries(test_eventloop pthread protobuf)
target_link_libraries(test_timer pthread protobuf)
target_link_libraries(test_thread_group pthread protobuf)
target_link_libraries(test_tcp_connection pthread protobuf)
target_link_libraries(test_rpc_server pthread protobuf)
target_link_libraries(test_rpc_client pthread protobuf)
