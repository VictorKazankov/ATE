#include "common.h"

#include <string>

#include "config/config.h"
#include "logger/logger_setup.h"

namespace common {

const config::Reader& Config() {
  static const std::string kDefaultConfigFile = VHAT_COMMON_DATA_PATH "/config.ini";
  static const config::Reader kConfig{kDefaultConfigFile};
  return kConfig;
}

void SetUpLogger() { logger::impl::SetUp(logger::impl::Config{common::Config()}); }

}  // namespace common
