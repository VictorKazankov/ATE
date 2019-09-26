#include "transport_adapter.h"

#include "interaction/ATE/tcp_server.h"

TransportAdapter::TransportAdapter(AteMessageAdapter& ate_message_adapter)
    : ate_message_adapter_(ate_message_adapter) {}

TransportAdapter::~TransportAdapter() {
  if (tcp_server_) {
    tcp_server_->Stop();
  }
}

void TransportAdapter::InitTcpAdapter(boost::asio::io_context& io_context, uint16_t port) {
  tcp_server_ = interaction::TcpServer::Create(io_context, port, ate_message_adapter_);
}

void TransportAdapter::Run() {
  if (tcp_server_) {
    tcp_server_->Start();
  }
}
