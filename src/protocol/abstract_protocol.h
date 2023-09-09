#ifndef SRC_PROTOCOL_ABSTRACT_PROTOCOL_H
#define SRC_PROTOCOL_ABSTRACT_PROTOCOL_H

#include <memory>


namespace seedrpc {

/**
 * 通过将 AbstractProtocol 继承自 std::enable_shared_from_this<AbstractProtocol>，
 * 您可以在 AbstractProtocol 类的成员函数中调用 shared_from_this() 来获得一个指向自
 * 身的 std::shared_ptr，而不会导致引用计数错误或悬挂指针的问题。
*/
struct AbstractProtocol : public std::enable_shared_from_this<AbstractProtocol> {
 public:
  typedef std::shared_ptr<AbstractProtocol> s_ptr;

  virtual ~AbstractProtocol() {}

 public:
  std::string m_msg_id;     // 请求号，唯一标识一个请求或者响应


};

}


#endif