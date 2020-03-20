#ifndef ATE_COMMON_MESSAGE_FACTORY_JSON_MESSAGES_H_
#define ATE_COMMON_MESSAGE_FACTORY_JSON_MESSAGES_H_

namespace common {
namespace jmsg {

constexpr auto kAttachToApplication = "AttachToApplication";
constexpr auto kWaitForObject = "WaitForObject";
constexpr auto kTapObject = "TapObject";
constexpr auto kTouchAndDrag = "TouchAndDrag";
constexpr auto kPressAndHold = "PressAndHold";
constexpr auto kPressRelease = "PressRelease";
constexpr auto kDisplayTypeChanged = "displayTypeChanged";
constexpr auto kChangeSyncIconDB = "ChangeSyncIconDB";
constexpr auto kChangeSyncMode = "ChangeSyncMode";
constexpr auto kReloadIconStorage = "ReloadIconStorage";
constexpr auto kLongPress = "LongPress";
constexpr auto kGetScreenshot = "GetScreenshot";
constexpr auto kGetText = "GetText";
constexpr auto kGetObjectsDataByPattern = "GetObjectsDataByPattern";
constexpr auto kGetImagesDiscrepancy = "GetImagesDiscrepancy";
constexpr auto kCaptureFrames = "CaptureFrames";

}  // namespace jmsg
}  // namespace common

#endif  // ATE_COMMON_MESSAGE_FACTORY_JSON_MESSAGES_H_
