#ifndef SRC_PROTOCOL_TINYPB_PROTOCOL_H
#define SRC_PROTOCOL_TINYPB_PROTOCOL_H
#include "src/protocol/abstract_protocol.h"
#include <string>

namespace seedrpc{

struct TinyPBProtocol : public AbstractProtocol {
 public:
  TinyPBProtocol(){}
  ~TinyPBProtocol() {}

 public:
  static char PB_START;  //起始字符
  static char PB_END;  //结束字符

  int32_t m_pk_len {0}; //包的长度 (4个字节) 【start ~ end】
  int32_t m_msg_id_len {0}; //存放消息id长度的字节长度 {4个字节}
  // msg_id
  int32_t m_method_name_len {0}; //存放方法名的长度的字节长度 {4个}
  //方法签名
  std::string m_method_name; //方法名  (service.fun)
  //错误码
  int32_t m_err_code {0}; // 错误码
  //错误信息
  int32_t m_err_info_len {0}; //存放错误信息的长度的字节长度(4)
  std::string m_err_info; //错误信息
  //message数据（序列化后）
  std::string m_pb_data; //protobuf序列化数据
  //校验码
  int32_t m_check_sum {0}; //校验码（用和表示）

  bool parse_success {false};

};


}

# endif