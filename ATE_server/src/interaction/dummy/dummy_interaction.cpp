#include "interaction/dummy/dummy_interaction.h"

#include "logger/logger.h"

namespace interaction {

void DummyInteraction::Tap(const int x, const int y) const { logger::trace("[dummy_interaction] Tap({}, {})", x, y); }

void DummyInteraction::Press(const int x, const int y) const {
  logger::trace("[dummy_interaction] Press({}, {})", x, y);
}

void DummyInteraction::Release(const int x, const int y) const {
  logger::trace("[dummy_interaction] Release({}, {})", x, y);
}

void DummyInteraction::Drag(const int x, const int y) const { logger::trace("[dummy_interaction] Drag({}, {})", x, y); }
}  // namespace interaction