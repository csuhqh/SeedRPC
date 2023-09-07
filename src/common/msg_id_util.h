#ifndef SRC_COMMON_MSGID_UTIL_H
#define SRC_COMMON_MSGID_UTIL_H

#include <string>


namespace seedrpc {

class MsgIDUtil {

 public:
  //生成字符串的id
  static std::string GenMsgID();

};

}


#endif