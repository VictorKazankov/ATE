#ifndef ATE_COMMON_MESSAGE_FACTORY_JSON_DEFINES_H_
#define ATE_COMMON_MESSAGE_FACTORY_JSON_DEFINES_H_

namespace common {
namespace jmsg {

// Header fields
constexpr auto kJsonRpc = "jsonrpc";
constexpr auto kRpcVersion = "2.0";
constexpr auto kId = "id";
constexpr auto kMethod = "method";
constexpr auto kParams = "params";
constexpr auto kResult = "result";

// Error fields
constexpr auto kError = "error";
constexpr auto kErrorCode = "code";
constexpr auto kErrorMessage = "message";
constexpr auto kErrorData = "data";

// Params names
constexpr auto kObjectName = "object_or_name";
constexpr auto kTimeoutMsec = "timeout_msec";
constexpr auto kAbscissa = "x";
constexpr auto kOrdinate = "y";
constexpr auto kVdpHost = "host";
constexpr auto kVdpPort = "port";
constexpr auto kWidth = "width";
constexpr auto kHight = "height";
constexpr auto kIsRunning = "is_running";

}  // namespace jmsg
}  // namespace common

#endif  // ATE_COMMON_MESSAGE_FACTORY_JSON_DEFINES_H_
