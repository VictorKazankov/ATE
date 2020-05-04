#ifndef ATE_CLIENT_CLIENT_CLI_ENUMS_H_
#define ATE_CLIENT_CLIENT_CLI_ENUMS_H_

#include "utils/squish_types.h"

namespace ATE {

// clang-format off

public enum class Modifier {
  None = common::squish::ModifierState::kNone,
  Alt = common::squish::ModifierState::kAlt,
  Control = common::squish::ModifierState::kControl,
  Shift = common::squish::ModifierState::kShift,
};

public enum class MouseButton {
  None = common::squish::MouseButton::kNone,
  LeftButton = common::squish::MouseButton::kLeftButton,
  MiddleButton = common::squish::MouseButton::kMiddleButton,
  RightButton = common::squish::MouseButton::kRightButton,
};

public enum class CollectionMode {
  None = common::squish::CollectionMode::kNone,
  Day = common::squish::CollectionMode::kDay,
  Night = common::squish::CollectionMode::kNight,
  Any = common::squish::CollectionMode::kAny,
};

// clang-format on

}  // namespace ATE

#endif  // ATE_CLIENT_CLIENT_CLI_ENUMS_H_
