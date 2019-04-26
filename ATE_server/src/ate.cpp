#include "ate.h"
#include "storage/json_storage.h"

namespace {
constexpr auto kConfigFile = VHAT_DATA_PATH "/config.ini";
const std::string kDBSection = "DB";
const std::string kPathOption = "Path";
const std::string kCollectionModeOption = "CollectionMode";
}  // namespace

ATE::ATE()
    : config_{kConfigFile},
      storage_{std::make_unique<storage::JsonStorage>(config_.GetString(kDBSection, kPathOption, ""),
                                                      config_.GetString(kDBSection, kCollectionModeOption, ""))} {
  if (!storage_->Connect()) {
    exit(EXIT_FAILURE);
  }
}

ATE::~ATE() { Close(); }

void ATE::Close() {
  // TODO
}
