#include "ate.h"

namespace {
constexpr auto kConfigFile = "config.ini";
}

ATE::ATE() : config_{kConfigFile} {
  // TODO
}

ATE::~ATE() { Close(); }

void ATE::Close() {
  // TODO
}
