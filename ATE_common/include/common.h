#ifndef ATE_COMMON_COMMON_H_
#define ATE_COMMON_COMMON_H_

#include <string>

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
 * @brief Get reference to application global Config instance
 * @return Reference to Config instance
 *
 * This function is thread-safe unless called simultaneously with SetUp().
 */
const config::Reader& Config();

/**
 * @fn SetUp
 *
 * @brief Function to init common facilities.
 * @param config_file Config
 *
 * This function initializes common facilities:
 *  - config with a desired config file
 *  - logger with a desired configured parameters in a LOG section of config file.
 *
 * This function is not thread-safe and must be called before any threads spawned.
 */
void SetUp(const std::string& config_file);

}  // namespace common

#endif  // ATE_COMMON_COMMON_H_
