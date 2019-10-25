#ifndef VHAT_ATE_COMMON_INCLUDE_UTILS_SQUISH_TYPES_CONVERTER_H_
#define VHAT_ATE_COMMON_INCLUDE_UTILS_SQUISH_TYPES_CONVERTER_H_

#include <string>

#include "squish_types.h"

namespace common {
namespace squish {

/**
 * @brief Convert CollectionMode from enum to string
 * @param mode Collection mode enum value
 * @return Collection mode in string
 */
std::string CollectionModeToStr(const CollectionMode& mode);

}  // namespace squish
}  // namespace common

#endif  // VHAT_ATE_COMMON_INCLUDE_UTILS_SQUISH_TYPES_CONVERTER_H_
