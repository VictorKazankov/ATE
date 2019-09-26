#ifndef ATE_COMMON_MESSAGE_FACTORY_JSON_MESSAGES_H_
#define ATE_COMMON_MESSAGE_FACTORY_JSON_MESSAGES_H_

namespace common {
namespace jmsg {

constexpr auto kAttachToApplication = "AttachToApplication";
constexpr auto kWaitForObject = "WaitForObject";
constexpr auto kTapObject = "TapObject";
constexpr auto kTouchAndDrag = "TouchAndDrag";
constexpr auto kDisplayTypeChanged = "displayTypeChanged";

}  // namespace jmsg
}  // namespace common

#endif  // ATE_COMMON_MESSAGE_FACTORY_JSON_MESSAGES_H_
