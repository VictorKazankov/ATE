#ifndef ATE_COMMON_MESSAGE_FACTORY_JSON_UTILS_H_
#define ATE_COMMON_MESSAGE_FACTORY_JSON_UTILS_H_

#include <string>

#include <jsoncpp/json/value.h>

namespace common {
namespace jmsg {

/**
 * @brief Parses and verify received message accordance with json formatted
 *  fills argument 'value' if parsing is okay
 *
 * @param json_message received message from client in string format
 * @param value reference to 'Json::Value' which should be filled if parsing is okay
 * @return true if parsing is okay false in bad case
 */
bool ParseJson(const std::string& json_message, Json::Value& value);

/**
 * @brief Check header of message
 *
 * @param value reference to 'Json::Value' for extract header of message and verify it
 * @return true if 'value' has field 'message' otherwise false
 */
bool CheckHeader(const Json::Value& value);

/**
 * @brief Check header type of message
 *
 * @param value reference to 'Json::Value' for extract header of message and verify header type
 * @return true if field 'message' is string otherwise false
 */
bool CheckHeaderType(const Json::Value& value);

/**
 * @brief Check scheme of message 'WaitForApplicationLaunch'
 *
 * @param value reference to 'Json::Value' for verification of json schema
 * @return true schema is correct otherwise false
 */
bool CheckAttachToApplicationRequest(const Json::Value& schema);

/**
 * @brief Check request scheme of message 'WaitForObject'
 *
 * @param value reference to 'Json::Value' for verification of json schema
 * @return true schema is correct otherwise false
 */
bool CheckWaitForObjectRequest(const Json::Value& schema);

/**
 * @brief Check request scheme of message 'TapObject'
 *
 * @param value reference to 'Json::Value' for verification of json schema
 * @return true schema is correct otherwise false
 */
bool CheckTapObjectRequest(const Json::Value& schema);

/**
 * @brief Check response scheme of message 'WaitForApplicationLaunch'
 *
 * @param value reference to 'Json::Value' for verification of json schema
 * @return true schema is correct otherwise false
 */
bool CheckAttachToApplicationResponse(const Json::Value& schema);

/**
 * @brief Check response scheme of message 'WaitForObject'
 *
 * @param value - reference to 'Json::Value' for verification of json schema
 * @return true - schema is correct otherwise false
 */
bool CheckWaitForObjectResponse(const Json::Value& schema);

}  // namespace jmsg
}  // namespace vhat_common

#endif  // ATE_COMMON_MESSAGE_FACTORY_JSON_UTILS_H_
