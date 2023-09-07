#ifndef SRC_NET_ABSTRACT_CODER_H
#define SRC_NET_ABSTRACT_CODER_H
#include <memory>
#include <vector>
#include "src/net/tcp/tcp_buffer.h"
#include "src/net/rpc/coder/abstract_protocol.h"

namespace seedrpc {

class AbstractCoder {
 public:
  typedef std::shared_ptr<AbstractCoder> s_ptr;
  // 序列化：将 message 对象转化为字节流，写入到 buffer
  virtual void encode(std::vector<AbstractProtocol::s_ptr>& messages, TcpBuffer::s_ptr out_buffer) = 0;

  // 反序列化：将 buffer 里面的字节流转换为 message 对象
  virtual void decode(std::vector<AbstractProtocol::s_ptr>& out_messages, TcpBuffer::s_ptr buffer) = 0;


};


}

#endif