#ifndef ATE_COMMON_UTILS_SQUISH_TYPES_H_
#define ATE_COMMON_UTILS_SQUISH_TYPES_H_

namespace common {
namespace squish {
/**
 * @brief ModifierState represents squish modifiers states
 * Named against accepted rules because of them uses in Python
 **/
enum class ModifierState { NONE, ALT, CONTROL, SHIFT };

/**
 * @brief MouseButton represents squish buttons
 * Named against accepted rules because of them uses in Python
 **/
enum class MouseButton { NONE, LEFT_BUTTON, MIDLE_BUTTON, RIGHT_BUTTON };

}  // namespace squish
}  // namespace common

#endif  // ATE_COMMON_UTILS_GEOMETRY_TYPES_H_
