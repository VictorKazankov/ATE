#include "interaction/ATE/tcp_session_handler.h"

#include <iostream>

#include "utils/logger.h"

namespace interaction {

void TcpSessionHandler::OnOpen(const std::shared_ptr<TcpConnection>& session) {
  session->Start();
}

void TcpSessionHandler::OnMessage(const std::shared_ptr<TcpConnection>& session, const std::string& message) {
  logger::info("[sessionhandler] receive message : {}", message);
  // TODO (MShvaiko@luxoft.com) : prepare response for the client
  session->Send(message + " : from server");
}

}  // namespace interaction
