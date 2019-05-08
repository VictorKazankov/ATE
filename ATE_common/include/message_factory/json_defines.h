#ifndef ATE_COMMON_MESSAGE_FACTORY_JSON_DEFINES_H_
#define ATE_COMMON_MESSAGE_FACTORY_JSON_DEFINES_H_

namespace vhat_common {
namespace jmsg {

constexpr auto kJsonRpc = "jsonrpc";
constexpr auto kRpcVersion = "2.0";
constexpr auto kId = "id";
constexpr auto kMethod = "method";
constexpr auto kParams = "params";
constexpr auto kResult = "result";

// Params names
constexpr auto kObjectName = "object_or_name";
constexpr auto kTimeout = "timeout_msec";
constexpr auto kAbscissa = "x";
constexpr auto kOrdinate = "y";
constexpr auto kVdpHost = "host";
constexpr auto kVdpPort = "port";
constexpr auto kWidth = "width";
constexpr auto kHight = "hight";
constexpr auto kIsRunning = "is_running";

}  // namespace jmsg
}  // namespace vhat_common

#endif  // ATE_COMMON_MESSAGE_FACTORY_JSON_DEFINES_H_
