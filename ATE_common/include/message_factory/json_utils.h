#ifndef ATE_COMMON_MESSAGE_FACTORY_JSON_UTILS_H_
#define ATE_COMMON_MESSAGE_FACTORY_JSON_UTILS_H_

#include <chrono>
#include <string>

#include "json/value.h"

#include "rpc_error.h"
#include "utils/squish_types.h"

namespace common {
namespace jmsg {

/**
 * @brief creates error object in jsonrpc format
 *
 * @param code - number that indicates the error type that occurred
 * @param message - string providing a short description of the error
 * @param data - primitive or Structured value that contains additional information about the error.
 * This may be omitted.

 * @return error as JSON object
 */
Json::Value CreateErrorObject(rpc::Error code, const char* message, Json::Value data = {});

/**
 * @brief Parses and verify received message accordance with json formatted
 * fills argument 'value' if parsing is okay
 *
 * @param json_message received message from client in string format
 * @param value reference to 'Json::Value' which should be filled if parsing is okay
 * @return true if parsing is okay false in bad case
 */
bool ParseJson(const std::string& json_message, Json::Value& value);

/**
 * @brief Parses JSON-RPC 2.0 request
 *
 * @param[in] json_message received from client
 *
 * @param[out] id - identifier established by the Client (not 0)
 * @param[out] method - string containing the name of the method to be invoke
 * @param[out] params - value that holds the parameter values to be used during the invocation of the method.
 * This member MAY be omitted.
 * @param[out] error - error object, null on successs
 */
void ParseJsonRpcRequest(const std::string& json_message, std::uint64_t& id, std::string& method, Json::Value& params,
                         Json::Value& error);

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
 * @brief Check scheme of message 'attachToApplication'
 *
 * @param value reference to 'Json::Value' for verification of json schema
 * @return true schema is correct otherwise false
 */
bool CheckAttachToApplicationRequest(const Json::Value& schema);

/**
 * @brief Extract params fot 'WaitForObject' method
 *
 * @param[in] params - structured value that holds the parameter values to be used
 * during the invocation of the 'WaitForObject' method
 *
 * @param[out] object_or_name
 * @param[out] timeout - object waiting duration
 * @param[out] error - error object, null on successs
 */
void ExtractWaitForObjectRequestParams(const Json::Value& params, std::string& object_or_name,
                                       std::chrono::milliseconds& timeout, Json::Value& error);

/**
 * @brief Extract params fot 'TapObject' method
 *
 * @param[in] params - structured value that holds the parameter values to be used
 * during the invocation of the 'TapObject' method
 *
 * @param[out] x - x coordinate
 * @param[out] y - y coordinate
 * @param[out] modifier_state - Modifier state
 * @param[out] mouse_button - Mouse button
 * @param[out] error - error object, null on successs
 */
void ExtractTapObjectRequestParams(const Json::Value& params, int& x, int& y, squish::ModifierState& modifier_state,
                                   squish::MouseButton& mouse_button, Json::Value& error);

/**
 * @brief Extract params for 'TouchAndDrag' method
 *
 * @param[in] params - structured value that holds the parameter values to be used
 * during the invocation of the 'TouchAndDrag' method
 *
 * @param[out] object_or_name
 * @param[out] x - x start coordinate
 * @param[out] y - y start coordinate
 * @param[out] dx - pixels to drag widget horizontally
 * @param[out] dy - pixels to drag widget vertically
 * @param[out] modifier_state - Modifier state
 * @param[out] error - error object, null on successs
 */
void ExtractTouchAndDragRequestParams(const Json::Value& params, std::string& object_or_name, int& x, int& y, int& dx,
                                      int& dy, squish::ModifierState& modifier_state, Json::Value& error);

void ExtractDisplayTypeChangedRequestParams(const Json::Value& params, int& x, int& y, Json::Value& error);

/**
 * @brief Check response scheme of message 'attachToApplication'
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

/**
 * @brief Extract parameters for ChangeSyncIconDB
 * @param params structured value that holds the parameter values to be used
 * during the invocation of the 'ChangeSyncIconDB' method
 * @param sync_version Sync version
 * @param sync_build_version Sync build version
 * @param error Error object, null on success
 */
void ExtractChangeSyncIconDBRequestParams(const Json::Value& params, std::string& sync_version,
                                          std::string& sync_build_version, Json::Value& error);

/**
 * @brief Extracts params for 'ChangeSyncMode' method
 * @param params - structured value that holds the parameter values to be used
 * during the invocation of the 'ChangeSyncMode' method
 * @param collection_mode - Collection mode
 * @param error - error object, null on success
 */
void ExtractChangeSyncModeRequestParams(const Json::Value& params, std::string& collection_mode, Json::Value& error);

}  // namespace jmsg
}  // namespace common

#endif  // ATE_COMMON_MESSAGE_FACTORY_JSON_UTILS_H_
