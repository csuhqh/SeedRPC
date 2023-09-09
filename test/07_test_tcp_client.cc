#include <assert.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>
#include <memory>
#include <unistd.h>
#include <google/protobuf/service.h>
#include "src/common/log.h"
#include "src/common/log.h"
#include "src/net/tcp_client.h"
#include "src/net/net_addr.h"
#include "src/protocol/coder/string_coder.h"
#include "src/protocol/abstract_protocol.h"
#include "src/protocol/coder/tinypb_coder.h"
#include "src/protocol/tinypb_protocol.h"
#include "src/net/net_addr.h"
#include "src/net/tcp_server.h"
#include "src/rpc/server/rpc_dispatcher.h"
#include "src/rpc/rpc_controller.h"
#include "src/rpc/client/rpc_channel.h"
#include "src/rpc/rpc_closure.h"

#include "src/rpc/pbs/order/order.pb.h"


void test_tcp_client() {
  //创建客户端
  seedrpc::IPNetAddr::s_ptr addr = std::make_shared<seedrpc::IPNetAddr>("127.0.0.1", 12345);
  seedrpc::TcpClient client(addr);
  //连接客户端
  client.connect([addr, &client]() {
    DEBUGLOG("conenct to [%s] success", addr->toString().c_str());
    //构造消息体
    std::shared_ptr<seedrpc::TinyPBProtocol> message = std::make_shared<seedrpc::TinyPBProtocol>();
    message->m_msg_id = "99998888";
    message->m_pb_data = "test pb data";
    //构造参数
    makeOrderRequest request;
    request.set_price(100);
    request.set_goods("apple");
    //序列化参数
    if (!request.SerializeToString(&(message->m_pb_data))) {
      ERRORLOG("serilize error");
      return;
    }
    //指明调用方法
    message->m_method_name = "Order.makeOrder";
    
    client.writeMessage(message, [request](seedrpc::AbstractProtocol::s_ptr msg_ptr) {
      DEBUGLOG("send message success, request[%s]", request.ShortDebugString().c_str());
    });


    client.readMessage("99998888", [](seedrpc::AbstractProtocol::s_ptr msg_ptr) {
      std::shared_ptr<seedrpc::TinyPBProtocol> message = std::dynamic_pointer_cast<seedrpc::TinyPBProtocol>(msg_ptr);
      DEBUGLOG("msg_id[%s], get response %s", message->m_msg_id.c_str(), message->m_pb_data.c_str());
      makeOrderResponse response;

      if(!response.ParseFromString(message->m_pb_data)) {
        ERRORLOG("deserialize error");
        return;
      }
      DEBUGLOG("get response success, response[%s]", response.ShortDebugString().c_str());
    });
  });
}

int main() {

  test_tcp_client();

  INFOLOG("test_rpc_channel end");

  return 0;
}