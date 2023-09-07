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
#include "src/client/tcp_client.h"
#include "src/net/tcp/net_addr.h"
#include "src/net/rpc/coder/string_coder.h"
#include "src/net/rpc/coder/abstract_protocol.h"
#include "src/net/rpc/coder/tinypb_coder.h"
#include "src/net/rpc/coder/tinypb_protocol.h"
#include "src/net/tcp/net_addr.h"
#include "src/server/tcp_server.h"
#include "src/net/rpc/rpc_dispatcher.h"
#include "src/net/rpc/rpc_controller.h"
#include "src/net/rpc/rpc_channel.h"
#include "src/net/rpc/rpc_closure.h"

#include "src/pbs/order.pb.h"



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

void test_rpc_channel() {

  NEWRPCCHANNEL("127.0.0.1:12345", channel);

  // std::shared_ptr<makeOrderRequest> request = std::make_shared<makeOrderRequest>();

  NEWMESSAGE(makeOrderRequest, request);
  NEWMESSAGE(makeOrderResponse, response);

  request->set_price(100);
  request->set_goods("apple");

  NEWRPCCONTROLLER(controller);
  controller->SetMsgId("99998888");
  controller->SetTimeout(10000);

  std::shared_ptr<seedrpc::RpcClosure> closure = std::make_shared<seedrpc::RpcClosure>(nullptr, [request, response, channel, controller]() mutable {
    if (controller->GetErrorCode() == 0) {
      INFOLOG("call rpc success, request[%s], response[%s]", request->ShortDebugString().c_str(), response->ShortDebugString().c_str());
      // 执行业务逻辑
      if (response->order_id() == "xxx") {
        // xx
      }
    } else {
      ERRORLOG("call rpc failed, request[%s], error code[%d], error info[%s]", 
        request->ShortDebugString().c_str(), 
        controller->GetErrorCode(), 
        controller->GetErrorInfo().c_str());
    }
  
    INFOLOG("now exit eventloop");
    // channel->getTcpClient()->stop();
    channel.reset();
  });

  CALLRPRC("127.0.0.1:12345", Order_Stub, makeOrder, controller, request, response, closure);

  // xxx
  // 协程
}

int main() {

  test_tcp_client();
  // test_rpc_channel();

  // INFOLOG("test_rpc_channel end");

  return 0;
}