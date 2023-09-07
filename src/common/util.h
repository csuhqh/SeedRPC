#ifndef SRC_COMMON_UTIL_H
#define SRC_COMMON_UTIL_H

#include <sys/types.h>
#include <unistd.h>

namespace seedrpc {

pid_t getPid();

pid_t getThreadId();

int64_t getNowMs();

time_t getTick();

int32_t getInt32FromNetByte(const char* buf);

}

#endif