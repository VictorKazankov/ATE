#ifndef ATE_COMMON_LOGGER_SETUP_H_
#define ATE_COMMON_LOGGER_SETUP_H_

namespace logger {

/**
 * @fn SetUp
 *
 * @brief Function to init logger.
 *
 * This function configures the spdlog logger system in a desired way.
 *
 * Some parameters can be configured in a LOG section of a default config file.
 */
void SetUp();

}  // namespace logger

#endif  // ATE_COMMON_LOGGER_SETUP_H_
