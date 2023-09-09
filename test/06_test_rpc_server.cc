#include <arpa/inet.h>
#include <assert.h>
#include <fcntl.h>
#include <google/protobuf/service.h>
#include <netinet/in.h>
#include <pthread.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include <memory>
#include <string>

#include "src/rpc/pbs/order/order.pb.h"

#include "src/common/log.h"
#include "src/protocol/abstract_protocol.h"
#include "src/protocol/coder/string_coder.h"
#include "src/protocol/coder/tinypb_coder.h"
#include "src/protocol/tinypb_protocol.h"
#include "src/rpc/server/rpc_dispatcher.h"
#include "src/net/net_addr.h"
#include "src/net/tcp_client.h"
#include "src/net/tcp_server.h"
#include "src/rpc/server/OrderImp.cc"

/**
message makeOrderRequest {
  int32 price = 1;
  string goods = 2;
}

message makeOrderResponse {
  int32 ret_code = 1;
  string res_info = 2;
  string order_id = 3;
}
*/

int main(int argc, char* argv[]) {
    //实现Services: 服务器实现Service的Imp类
    std::shared_ptr<OrderImpl> service = std::make_shared<OrderImpl>();
    //注册service
    seedrpc::RpcDispatcher::GetRpcDispatcher()->registerService(service);
    //设置IP地址
    seedrpc::IPNetAddr::s_ptr addr = std::make_shared<seedrpc::IPNetAddr>("127.0.0.1", 12345);
    //开启tcp服务器
    seedrpc::TcpServer tcp_server(addr);
    tcp_server.start();
    return 0;
}