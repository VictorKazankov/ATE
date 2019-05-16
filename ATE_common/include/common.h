#ifndef ATE_COMMON_COMMON_H_
#define ATE_COMMON_COMMON_H_

#include "config/config.h"

/**
 * @namespace common
 *
 * @brief Namespace for common facilities
 */
namespace common {

/**
 * @fn Config
 *
 * @brief Get reference to Config instance
 * @return reference to Config instance
 *
 * Meyers singleton for lazy thread-safe init of Config instance
 * and  obtaining its reference;
 */
const config::Reader& Config();

}  // namespace common

#endif  // ATE_COMMON_COMMON_H_
