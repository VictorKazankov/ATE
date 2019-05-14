#ifndef ATE_SERVER_TCP_SESSION_HANDLER_H_
#define ATE_SERVER_TCP_SESSION_HANDLER_H_

#include "server/session_handler.h"

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
  ~TcpSessionHandler() override = default;

  /**
   * @brief Start interaction between client and server calls TcpConnectionPtr->Start()
   * @param session - Pointer to has been accepted connection
   */
  void OnOpen(const std::shared_ptr<TcpConnection>& session) override;

  /**
   * @brief Gets messages from client, prepare and send response
   * @param session - Pointer to connection with client
   * @param message - Received message from client connection
   */
  void OnMessage(const std::shared_ptr<TcpConnection>& session, const std::string& message) override;
};

}  // namespace interaction

#endif  // ATE_SERVER_TCP_SESSION_HANDLER_H_
