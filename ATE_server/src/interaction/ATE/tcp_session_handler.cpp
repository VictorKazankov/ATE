#include "interaction/ATE/tcp_session_handler.h"

#include "ate_message_adapter.h"

namespace interaction {

TcpSessionHandler::TcpSessionHandler(AteMessageAdapter& ate_adapter) : ate_message_adapter_(ate_adapter) {}

void TcpSessionHandler::OnOpen(std::shared_ptr<Connection> session) { session->Start(); }

void TcpSessionHandler::OnMessage(std::shared_ptr<Connection> session, const std::string& message) {
  auto response = ate_message_adapter_.OnMessage(message);
  session->Send(response);
}

}  // namespace interaction
