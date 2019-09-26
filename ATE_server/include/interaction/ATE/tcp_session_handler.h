#ifndef ATE_SERVER_INTERACTION_ATE_TCP_SESSION_HANDLER_H_
#define ATE_SERVER_INTERACTION_ATE_TCP_SESSION_HANDLER_H_

#include <string>

#include "ate_message_adapter.h"
#include "interaction/ATE/session_handler.h"

namespace interaction {

/**
 * @class TcpSessionHandler
 *
 * @brief Class for handlinc tcp connections
 *
 * Class receives request and send responses via method 'OnMessage' over TCP protocol
 *
 */
class TcpSessionHandler : public SessionHandler {
 public:
  TcpSessionHandler(AteMessageAdapter& ate_adapter);
  ~TcpSessionHandler() override = default;

  /**
   * @brief Start interaction between client and server calls TcpConnectionPtr->Start()
   * @param session - Handle for the accepted connection
   */
  void OnOpen(std::shared_ptr<Connection> session) override;

  /**
   * @brief Gets messages from client, prepare and send response
   * @param session - Connection with client
   * @param message - Received message from client connection
   */
  void OnMessage(std::shared_ptr<Connection> session, const std::string& message) override;

 private:
  AteMessageAdapter& ate_message_adapter_;
};

}  // namespace interaction

#endif  // ATE_SERVER_INTERACTION_ATE_TCP_SESSION_HANDLER_H_
