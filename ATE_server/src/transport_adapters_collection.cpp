#include "transport_adapters_collection.h"

#include "interaction/ATE/tcp_connection_manager.h"

TransportAdaptersCollection::TransportAdaptersCollection(AteMessageAdapter& ate_message_adapter)
    : ate_message_adapter_(ate_message_adapter) {}

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
  tcp_server_ = interaction::TcpConnectionManager::Create(io_context, port, ate_message_adapter_);
}
