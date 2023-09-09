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

#common and net and rector
aux_source_directory(${PROJ_PATH}/src/common COMMON_SRCS)
aux_source_directory(${PROJ_PATH}/src/net NET_SRCS)
aux_source_directory(${PROJ_PATH}/src/reactor RECTOR_SRCS)
list(APPEND ALL_SRCS ${COMMON_SRCS} ${NET_SRCS} ${RECTOR_SRCS})
# 协议
aux_source_directory(${PROJ_PATH}/src/protocol PROTOCOL_SRCS)
aux_source_directory(${PROJ_PATH}/src/protocol/coder PROTOCOL_CODER_SRCS)
list(APPEND ALL_SRCS ${PROTOCOL_SRCS} ${PROTOCOL_CODER_SRCS})
# # rpc
aux_source_directory(${PROJ_PATH}/src/rpc RPC_SRCS)
aux_source_directory(${PROJ_PATH}/src/rpc/pbs/order RPC_PBS_ORDER_SRCS)  #service
aux_source_directory(${PROJ_PATH}/src/rpc/client CLIENT_SRCS)
aux_source_directory(${PROJ_PATH}/src/rpc/server SERVER_SRCS)
list(APPEND ALL_SRCS ${RPC_SRCS} ${RPC_PBS_ORDER_SRCS} ${CLIENT_SRCS} ${SERVER_SRCS})





add_executable(test_log test/01_test_log.cc ${ALL_SRCS})
add_executable(test_eventloop test/02_test_eventloop.cc ${ALL_SRCS})
add_executable(test_timer test/03_test_timer.cc ${ALL_SRCS})
add_executable(test_thread_group test/04_test_thread_group.cc ${ALL_SRCS} )
add_executable(test_tcp_connection test/05_test_tcp_connection.cc ${ALL_SRCS} )
add_executable(test_rpc_server test/06_test_rpc_server.cc ${ALL_SRCS})
add_executable(test_tcp_client test/07_test_tcp_client.cc ${ALL_SRCS})
add_executable(test_rpc_client test/08_test_rpc_client.cc ${ALL_SRCS})

target_link_libraries(test_log pthread protobuf) #需要添加静态链接库 libpthread.a
target_link_libraries(test_eventloop pthread protobuf)
target_link_libraries(test_timer pthread protobuf)
target_link_libraries(test_thread_group pthread protobuf)
target_link_libraries(test_tcp_connection pthread protobuf)
target_link_libraries(test_rpc_server pthread protobuf)
target_link_libraries(test_tcp_client pthread protobuf)
target_link_libraries(test_rpc_client pthread protobuf)
