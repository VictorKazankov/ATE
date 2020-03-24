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
constexpr auto kAbscissaDrag = "dx";
constexpr auto kOrdinateDrag = "dy";
constexpr auto kModifierState = "modifier_state";
constexpr auto kMouseButton = "button";
constexpr auto kHost = "host";
constexpr auto kPort = "port";
constexpr auto kWidth = "width";
constexpr auto kHeight = "height";
constexpr auto kIsRunning = "is_running";
constexpr auto kSyncVersion = "sync_version";
constexpr auto kSyncBuildVersion = "sync_build_version";
constexpr auto kSyncCollectionMode = "sync_collection_mode";
constexpr auto kLocation = "location";
constexpr auto kFileName = "filename";
constexpr auto kText = "text";
// Object data
constexpr auto kName = "name";
constexpr auto kParentScreen = "parent_screen";
constexpr auto kXTopLeft = "x_top_left";
constexpr auto kYTopLeft = "y_top_left";
constexpr auto kXBottomRight = "x_bottom_right";
constexpr auto kYBottomRight = "y_bottom_right";
constexpr auto kParentWidth = "parent_width";
constexpr auto kParentHeight = "parent_height";
// Image discrepancy
constexpr auto kIconPathFirst = "icon_path_first";
constexpr auto kIconPathSecond = "icon_path_second";
constexpr auto kDiscrepancyPercent = "discrepancy_percent";

}  // namespace jmsg
}  // namespace common

#endif  // ATE_COMMON_MESSAGE_FACTORY_JSON_DEFINES_H_
