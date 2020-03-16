#ifndef ATE_COMMON_UTILS_SQUISH_TYPES_H_
#define ATE_COMMON_UTILS_SQUISH_TYPES_H_

namespace common {
namespace squish {
/**
 * @brief ModifierState represents squish modifiers states
 **/
enum class ModifierState { kNone, kAlt, kControl, kShift };

/**
 * @brief MouseButton represents squish buttons
 **/
enum class MouseButton { kNone, kLeftButton, kMiddleButton, kRightButton };

/**
 * @brief CollectionMode represents collection mode
 **/
enum class CollectionMode { kNone, kDay, kNight, kAny };

}  // namespace squish
}  // namespace common

#endif  // ATE_COMMON_UTILS_GEOMETRY_TYPES_H_
