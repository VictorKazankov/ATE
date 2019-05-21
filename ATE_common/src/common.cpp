#include "common.h"

#include <cassert>
#include <memory>

#include "logger/logger.h"
#include "logger/logger_setup.h"

namespace common {

namespace {
std::unique_ptr<const config::Reader> kConfig;
}  // namespace

const config::Reader& Config() {
  assert(kConfig);
  return *kConfig;
}

void SetUp(const std::string& config_file) {
  kConfig = std::make_unique<const config::Reader>(config_file);
  logger::SetUp(*kConfig);
}

}  // namespace common
