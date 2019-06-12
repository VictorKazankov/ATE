#ifndef ATE_SERVER_INTERACTION_ATE_TCP_SESSION_HANDLER_H_
#define ATE_SERVER_INTERACTION_ATE_TCP_SESSION_HANDLER_H_

#include <cstdint>
#include <string>
#include <unordered_map>

#include <boost/asio/io_context.hpp>

#include "ate.h"
#include "interaction/ATE/session_handler.h"

namespace Json {
class Value;
}  // namespace Json

namespace utils {
class ATEServerAppContext;
}  // namespace utils

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
  TcpSessionHandler(const utils::ATEServerAppContext& app_ctx, boost::asio::io_context& io_ctx);
  ~TcpSessionHandler() override = default;

  /**
   * @brief Start interaction between client and server calls TcpConnectionPtr->Start()
   * @param session - Handle for the accepted connection
   */
  void OnOpen(TcpConnection& session) override;

  /**
   * @brief Gets messages from client, prepare and send response
   * @param session - Connection with client
   * @param message - Received message from client connection
   */
  void OnMessage(TcpConnection& session, const std::string& message) override;

 private:
  void SendResponse(TcpConnection& session, std::uint64_t id, Json::Value result_or_error, bool is_result);

  typedef std::pair<Json::Value, bool> (TcpSessionHandler::*MessageHandlerFunction)(const Json::Value& params);

  MessageHandlerFunction GetHandler(const std::string& method) const noexcept;

  /**
   * @brief Handler for WaitForObject
   * @param params - WaitForObject params
   */
  std::pair<Json::Value, bool> HandleWaitForObject(const Json::Value& params);

  /**
   * @brief Handler for TapObject
   * @param params - TapObject params
   */
  std::pair<Json::Value, bool> HandleTapObject(const Json::Value& params);

  /**
   * @brief Handler for unknown method
   * @param params - params from client message
   */
  std::pair<Json::Value, bool> HandleUnknownMethod(const Json::Value& params);

  ATE ate_;
  const std::unordered_map<std::string, MessageHandlerFunction> handler_map_;
};

}  // namespace interaction

#endif  // ATE_SERVER_INTERACTION_ATE_TCP_SESSION_HANDLER_H_
