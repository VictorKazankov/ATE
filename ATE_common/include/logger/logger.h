#ifndef ATE_COMMON_LOGGER_LOGGER_H_
#define ATE_COMMON_LOGGER_LOGGER_H_

#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>

/**
 * @namespace logger
 *
 * @brief Aliases for spdlog
 */
namespace logger {

using spdlog::critical;
using spdlog::debug;
using spdlog::error;
using spdlog::info;
using spdlog::trace;
using spdlog::warn;

}  // namespace logger

#endif  // ATE_COMMON_LOGGER_LOGGER_H_
