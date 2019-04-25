#include "ate.h"

namespace {
constexpr auto kConfigFile = VHAT_DATA_PATH "/config.ini";
}  // namespace

ATE::ATE() : config_{kConfigFile} {
  // TODO
}

ATE::~ATE() { Close(); }

void ATE::Close() {
  // TODO
}
