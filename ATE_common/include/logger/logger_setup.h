#ifndef ATE_COMMON_LOGGER_SETUP_H_
#define ATE_COMMON_LOGGER_SETUP_H_

#include <cstddef>

#include <spdlog/spdlog.h>

#include "config/config.h"

namespace logger {
namespace impl {

struct Config {
  explicit Config(const config::Reader& config);

  bool console_enabled{false};
  spdlog::level::level_enum console_level{spdlog::level::off};
  bool file_enabled{false};
  spdlog::level::level_enum file_level{spdlog::level::off};
  std::size_t max_files{0};
  std::size_t max_file_size{0};
};

void SetUp(const Config& config);

}  // namespace impl
}  // namespace logger

#endif  // ATE_COMMON_LOGGER_SETUP_H_
