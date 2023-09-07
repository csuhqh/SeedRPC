#include "src/server/tcp_server.h"
#include "src/net/tcp/net_addr.h"



int main(int argc, char const *argv[])
{
  seedrpc::NetAddr::s_ptr ipaddr = std::make_shared<seedrpc::IPNetAddr>("127.0.0.1:12345");
  seedrpc::TcpServer::s_ptr server = std::make_shared<seedrpc::TcpServer>(ipaddr);
  server->start();
  return 0;
}
