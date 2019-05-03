#include "server/tcp_session_handler.h"

#include <iostream>

namespace interaction {

void TcpSessionHandler::OnOpen(const std::shared_ptr<TcpConnection>& session) { session->Start(); }

void TcpSessionHandler::OnMessage(const std::shared_ptr<TcpConnection>& session, const std::string& message) {
  std::clog << "[INFO] receive message : " << message << std::endl;
  // TODO (MShvaiko@luxoft.com) : prepare response for the client
  session->Send(message + " : from server");
}

}  // namespace interaction