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


void test_rpc_channel() {
  seedrpc::NetAddr::s_ptr ipaddr = std::make_shared<seedrpc::IPNetAddr>("127.0.0.1:12345"); 
  seedrpc::RpcChannel::s_ptr channel = std::make_shared<seedrpc::RpcChannel>(ipaddr);
  //makeOrder方法的参数与返回值
  std::shared_ptr<makeOrderRequest> request = std::make_shared<makeOrderRequest>();
  std::shared_ptr<makeOrderResponse> response = std::make_shared<makeOrderResponse>();
  //给参数赋值
  request->set_price(100);
  request->set_goods("apple");
  //创建一个controller
  seedrpc::RpcController::s_ptr controller = std::make_shared<seedrpc::RpcController>();
  controller->SetMsgId("99998888");
  controller->SetTimeout(10000);
  //创建一个closure
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
  channel->Init(controller, request, response, closure);
  //对于客户端来说，每一个service都有一个Stub.
  Order_Stub(channel.get()).makeOrder(controller.get(), request.get(), response.get(), closure.get());
  // stub_name(channel.get()).method_name(controller.get(), request.get(), response.get(), closure.get());
  // CALLRPRC("127.0.0.1:12345", Order_Stub, makeOrder, controller, request, response, closure);
  // channel->CallMethod();

  // xxx
  // 协程
}

int main() {

  // test_tcp_client();
  test_rpc_channel();

  // INFOLOG("test_rpc_channel end");

  return 0;
}