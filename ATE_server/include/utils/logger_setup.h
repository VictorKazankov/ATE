#ifndef ATE_SERVER_UTILS_LOGGER_SETUP_H_
#define ATE_SERVER_UTILS_LOGGER_SETUP_H_

#include <spdlog/common.h>

#include "config/config.h"

namespace logger {

/**
 * \class Setup
 *
 * \brief Class to configure logger.
 *
 * This class configures the spdlog logger system in a desired way.
 *
 * Some parameters can be configured in a LOG section of config file.
 *
 */
class Setup {
 public:
  /// Create a logger Setup with a desired config
  Setup(const config::Reader& config_reader);

 private:
  struct Config {
    Config(const config::Reader& config_reader);

    bool console_enabled{false};
    spdlog::level::level_enum console_level{spdlog::level::off};
    bool file_enabled{false};
    spdlog::level::level_enum file_level{spdlog::level::off};
    std::size_t max_files{0};
    std::size_t max_file_size{0};
  };

  const Config config_;
};

}  // namespace logger

#endif  // ATE_SERVER_UTILS_LOGGER_SETUP_H_
