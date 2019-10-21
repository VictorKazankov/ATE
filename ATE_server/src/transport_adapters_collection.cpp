#include "transport_adapters_collection.h"

#include "interaction/ATE/tcp_connection_manager.h"
#include "interaction/DBUS/dbus_connection_manager.h"

TransportAdaptersCollection::TransportAdaptersCollection(IMessageAdapter& ate_message_adapter)
    : ate_message_adapter_(ate_message_adapter) {}

TransportAdaptersCollection::~TransportAdaptersCollection() { Stop(); }

void TransportAdaptersCollection::Run() {
  for (auto manager : connection_managers_) {
    manager->Start();
  }
}

void TransportAdaptersCollection::Stop() {
  for (auto manager : connection_managers_) {
    manager->Stop();
  }
}

void TransportAdaptersCollection::InitTcpConnectionManager(boost::asio::io_context& io_context, uint16_t port) {
  connection_managers_.emplace_back(interaction::TcpConnectionManager::Create(io_context, port, ate_message_adapter_));
}

void TransportAdaptersCollection::InitDbusConnectionManager(const std::string& interface) {
  connection_managers_.emplace_back(
      std::make_shared<interaction::DBusConnectionManager>(interface, ate_message_adapter_));
}
