#ifndef ATE_SERVER_ATE_MESSAGE_ADAPTER_H_
#define ATE_SERVER_ATE_MESSAGE_ADAPTER_H_

#include <cstdint>
#include <string>
#include <unordered_map>

#include <boost/asio/io_context.hpp>

#include "ate.h"

namespace Json {
class Value;
}  // namespace Json

/**
 * @class AteMessageAdapter
 * @brief Deserialize of messages with json rpc 2.0 representation and calls handler methods of ATE accordance with
 *  method type
 **/
class AteMessageAdapter {
 public:
  /**
   * @brief Default constructor
   * @param ate reference to instance of ATE
   */
  explicit AteMessageAdapter(ATE& ate);

  /**
   * @brief Receive string message requests in json rpc 2.0 representation parse
   * message and calls handler accordance with method type
   * @param message string request with json rpc 2.0 representation
   * @return response accordance with handler in json rpc 2.0
   */
  std::string OnMessage(const std::string& message);

 private:
  typedef std::pair<Json::Value, bool> (AteMessageAdapter::*MessageHandlerFunction)(const Json::Value& params);

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
   * @brief Handler for TouchAndDrag
   * @param params - TouchAndDrag params
   * @return pair of Json structure (handle result in case of success and error results in case of failure) and bool
   * (for easier verification of the error)
   */
  std::pair<Json::Value, bool> HandleTouchAndDrag(const Json::Value& params);

  /**
   * @brief Handler for unknown method
   * @param params - params from client message
   */
  std::pair<Json::Value, bool> HandleUnknownMethod(const Json::Value& params);

  ATE& ate_;
  const std::unordered_map<std::string, MessageHandlerFunction> handler_map_;
};

#endif  // ATE_SERVER_ATE_MESSAGE_ADAPTER_H_
