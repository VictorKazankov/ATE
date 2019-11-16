#ifndef ATE_SERVER_INCLUDE_UTILS_ENV_CONFIG_H_
#define ATE_SERVER_INCLUDE_UTILS_ENV_CONFIG_H_

#include <string>

namespace utils {

/**
 * @brief An enum with available settings options for parsing from environment variables
 */
enum class EnvSettings {
  kSpiDeviceAddress,
  kGpioVideoStatusPath,
};

/**
 * @brief Get Specific settings from environment variables
 * @param setting Setting for parsing from environment variables
 * @return Setting variable. By default return empty string
 */
std::string GetEnvSettings(const EnvSettings& setting);

}  // namespace utils

#endif  // ATE_SERVER_INCLUDE_UTILS_ENV_CONFIG_H_
