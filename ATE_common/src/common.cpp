#include "common.h"

#include <string>

#include "config/config.h"

namespace common {

const config::Reader& Config() {
  static const std::string kDefaultConfigFile = VHAT_DATA_PATH "/config.ini";
  static const config::Reader kConfig{kDefaultConfigFile};
  return kConfig;
}

}  // namespace common
