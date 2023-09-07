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

#include "src/pbs/order.pb.h"

#include "src/common/log.h"
#include "src/net/rpc/coder/abstract_protocol.h"
#include "src/net/rpc/coder/string_coder.h"
#include "src/net/rpc/coder/tinypb_coder.h"
#include "src/net/rpc/coder/tinypb_protocol.h"
#include "src/net/rpc/rpc_dispatcher.h"
#include "src/net/tcp/net_addr.h"
#include "src/client/tcp_client.h"
#include "src/server/tcp_server.h"


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

class OrderImpl : public Order {
public:
    void makeOrder(google::protobuf::RpcController* controller,
                   const ::makeOrderRequest* request,
                   ::makeOrderResponse* response,
                   ::google::protobuf::Closure* done) {
        // APPDEBUGLOG("start sleep 5s");
        sleep(5);
        // APPDEBUGLOG("end sleep 5s");

        if (request->price() < 10) {
            response->set_ret_code(-1);
            response->set_res_info("short balance");
            return;
        }
        response->set_order_id("20230514");
        // APPDEBUGLOG("call makeOrder success");
        if (done) {
            done->Run();
            delete done;
            done = NULL;
        }
    }
};

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