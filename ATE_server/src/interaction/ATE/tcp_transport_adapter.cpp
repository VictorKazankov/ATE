#include "interaction/ATE/tcp_transport_adapter.h"

#include "ate_message_adapter.h"

namespace interaction {

TcpTransportAdapter::TcpTransportAdapter(AteMessageAdapter& ate_adapter) : ate_message_adapter_(ate_adapter) {}

void TcpTransportAdapter::OnOpen(std::shared_ptr<Connection> session) { session->Start(); }

void TcpTransportAdapter::OnMessage(std::shared_ptr<Connection> session, const std::string& message) {
  auto response = ate_message_adapter_.OnMessage(message);
  session->Send(response);
}

}  // namespace interaction
