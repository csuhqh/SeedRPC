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
#include "src/rpc/pbs/order/order.pb.h"
#include <unistd.h>



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