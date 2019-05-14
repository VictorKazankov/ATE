#ifndef ATE_SERVER_SESSION_HANDLER_H_
#define ATE_SERVER_SESSION_HANDLER_H_

#include <memory>
#include <string>

#include "server/tcp_connection.h"

namespace interaction {

class TcpConnection;

/**
 * @class SessionHandler
 *
 * @brief Interface class for session handling (interaction between client/server)
 *
 * Class receives request and send responses via method 'OnMessage'
 *
 */
class SessionHandler {
 public:
  virtual ~SessionHandler() = default;

  /**
   * @brief Start interaction between client and server calls TcpConnectionPtr->Start()
   * @param session - Pointer to has been accepted connection
   */
  virtual void OnOpen(const std::shared_ptr<TcpConnection>& session) = 0;

  /**
   * @brief Gets messages from client, prepare and send response
   * @param session - Pointer to connection with client
   * @param message - Received message from client connection
   */
  virtual void OnMessage(const std::shared_ptr<TcpConnection>& session, const std::string& message) = 0;
};

}  // namespace interaction

#endif  // ATE_SERVER_SESSION_HANDLER_H_
