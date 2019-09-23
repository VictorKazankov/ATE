#include "transport_adapters_collection.h"

#include "interaction/ATE/tcp_server.h"

TransportAdaptersCollection::~TransportAdaptersCollection() {
  if (tcp_server_) {
    tcp_server_->Stop();
  }
}

void TransportAdaptersCollection::Run() {
  if (tcp_server_) {
    tcp_server_->Start();
  }
}

void TransportAdaptersCollection::InitTcpConnectionManager(boost::asio::io_context& io_context, uint16_t port) {
  tcp_server_ = interaction::TcpServer::Create(io_context, port);
}
