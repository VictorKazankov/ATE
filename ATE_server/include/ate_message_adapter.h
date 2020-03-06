#ifndef ATE_SERVER_ATE_MESSAGE_ADAPTER_H_
#define ATE_SERVER_ATE_MESSAGE_ADAPTER_H_

#include <cstdint>
#include <mutex>
#include <string>
#include <unordered_map>

#include <boost/asio/io_context.hpp>

#include "ate.h"

namespace Json {
class Value;
}  // namespace Json

/**
 * @brief Interface for message adapter
 */
class IMessageAdapter {
 public:
  virtual std::string OnMessage(const std::string& message) = 0;
  virtual ~IMessageAdapter() = default;
};

/**
 * @class AteMessageAdapter
 * @brief Deserialize of messages with json rpc 2.0 representation and calls handler methods of ATE accordance with
 *  method type
 **/
class AteMessageAdapter : public IMessageAdapter {
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
  std::string OnMessage(const std::string& message) override;

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
   * @brief Handler for LongPress
   * @param params - LongPress params
   * @return pair of Json structure (handle result in case of success and error results in case of failure) and bool
   * (for easier verification of the error)
   */
  std::pair<Json::Value, bool> HandleLongPress(const Json::Value& params);

  /**
   * @brief Handler for TouchAndDrag
   * @param params - TouchAndDrag params
   * @return pair of Json structure (handle result in case of success and error results in case of failure) and bool
   * (for easier verification of the error)
   */
  std::pair<Json::Value, bool> HandleTouchAndDrag(const Json::Value& params);

  /**
   * @brief Handler for PressAndHold
   * @param params - PressAndHold params
   * @return pair of Json structure (handle result in case of success and error results in case of failure) and bool
   * (for easier verification of the error)
   */
  std::pair<Json::Value, bool> HandlePressAndHold(const Json::Value& params);

  /**
   * @brief Handler for PressRelease
   * @param params - PressRelease params
   * @return pair of Json structure (handle result in case of success and error results in case of failure) and bool
   * (for easier verification of the error)
   */
  std::pair<Json::Value, bool> HandlePressRelease(const Json::Value& params);

  /**
   * @brief Handler for DisplayTypeChanged
   * @param params - DisplayTypeChanged params
   */
  std::pair<Json::Value, bool> HandleDisplayTypeChanged(const Json::Value& params);

  /**
   * @brief Change sync version
   * @param params Params from client message
   * @return pair of Json structure (handle result in case of success and error results in case of failure) and bool
   * (for easier verification of the error)
   */
  std::pair<Json::Value, bool> HandleChangeSyncIconDB(const Json::Value& params);

  /**
   * @brief Handler for ChangeSyncMode
   * @param params - ChangeSyncMode params
   * @return pair of Json structure (handle result in case of success and error results in case of failure) and bool
   * (for easier verification of the error)
   */
  std::pair<Json::Value, bool> HandleChangeSyncMode(const Json::Value& params);

  /**
   * @brief Handler for ReloadIconStorage
   * @param params ReloadIconStorage params
   */
  std::pair<Json::Value, bool> HandleReloadIconStorage(const Json::Value& params);

  /**
   * @brief Handler for GetScreenshot
   * @param params GetScreenshot params
   * @return pair of Json structure (handle result in case of success and error results in case of failure) and bool
   * (for easier verification of the error)
   */
  std::pair<Json::Value, bool> HandleGetScreenshot(const Json::Value& params);

  /**
   * @brief Handler for GetText
   * @param params GetText params
   * @return pair of Json structure (handle result in case of success and error results in case of failure) and bool
   * (for easier verification of the error)
   */
  std::pair<Json::Value, bool> HandleGetText(const Json::Value& params);

  /**
   * @brief Handler for GetObjectsDataByPattern
   * @param params GetObjectsDataByPattern params
   * @return pair of Json structure (handle result in case of success and error results in case of failure) and bool
   * (for easier verification of the error)
   */
  std::pair<Json::Value, bool> HandleGetObjectsDataByPattern(const Json::Value& params);

  /**
   * @brief Handler for ImagesDiscrepancy
   * @param params ImagesDiscrepancy params
   * @return Pair of Json structure (handle result in case of success and error results in case of failure) and bool
   *         (for easier verification of the error)
   */
  std::pair<Json::Value, bool> ImagesDiscrepancy(const Json::Value& params);

  /**
   * @brief Handler for unknown method
   * @param params - params from client message
   */
  std::pair<Json::Value, bool> HandleUnknownMethod(const Json::Value& params);

  /**
   * @brief Checking result after change sync configuration parameters
   * @param error Error code after changing sync configuration parameters
   * @return Error if exist error after change sync configuration parameters, empty value otherwise
   */
  Json::Value CheckChangeSyncConfigurationResult(const adapter::DBManagerError& error);

 private:
  std::mutex on_message_guard_;
  ATE& ate_;
  const std::unordered_map<std::string, MessageHandlerFunction> handler_map_;
};

#endif  // ATE_SERVER_ATE_MESSAGE_ADAPTER_H_
