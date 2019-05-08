#ifndef ATE_SERVER_INTERACTION_ATE_TCP_SESSION_HANDLER_H_
#define ATE_SERVER_INTERACTION_ATE_TCP_SESSION_HANDLER_H_

#include <functional>
#include <string>
#include <unordered_map>

#include <jsoncpp/json/value.h>

#include "session_handler.h"

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
  TcpSessionHandler();
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

 private:
  /**
   * @brief Handler method for meessage 'AttachToApplication'
   * @param session - Pointer to connection with client
   * @param json_message - Json::Value json structure using for extract message payload
   */
  static void HandleAttachToApplication(const std::shared_ptr<TcpConnection>& session,
                                             const Json::Value& json_message);

  /**
   * @brief Handler method for meessage 'WaitForObject'
   * @param session - Pointer to connection with client
   * @param json_message - Json::Value json structure using for extract message payload
   */
  static void HandleWaitForObject(const std::shared_ptr<TcpConnection>& session, const Json::Value& json_message);

  /**
   * @brief Handler method for meessage 'TapObject'
   * @param session - Pointer to connection with client
   * @param json_message - Json::Value json structure using for extract message payload
   */
  static void HandleTapObject(const std::shared_ptr<TcpConnection>& session, const Json::Value& json_message);

  using MessageHandlerFunction =
      std::function<void(const std::shared_ptr<TcpConnection>& session, const Json::Value& json_message)>;
  std::unordered_map<std::string, MessageHandlerFunction> handler_map_;
};

}  // namespace interaction

#endif  // ATE_SERVER_INTERACTION_ATE_TCP_SESSION_HANDLER_H_
