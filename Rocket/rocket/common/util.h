#ifndef ROCKET_COMMON_UTIL_H
#define ROCKET_COMMON_UTIL_H

#include <sys/types.h>
#include <unistd.h>

namespace rocket {

pid_t getPid();

pid_t getThreadId();

int64_t getNowMs();

time_t getTick();

int32_t getInt32FromNetByte(const char* buf);

}

#endif