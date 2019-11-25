#include "squish/application_context.h"

#include "logger/logger.h"

using namespace squish;

void ApplicationContext::Attach(std::unique_ptr<interaction::Interaction>&& ate_interaction) {
  this->ate_interaction = std::move(ate_interaction);
}

bool ApplicationContext::IsRunning() const {
  if (ate_interaction && ate_interaction->IsConnectionOpened()) return true;

  return false;
}

void ApplicationContext::Detach() {}

Object ApplicationContext::SendCommand(interaction::Method method, const std::string& command) {
  if (!IsRunning()) {
    logger::error("[application context] ATE server wasn't connected. Please perform 'attachToApplication()' first");
    throw std::runtime_error("Connection to ATE server is not established");
  }
  return ate_interaction->SendCommand(method, command);
}

std::string ApplicationContext::host() const { return ate_interaction->host(); }

std::string ApplicationContext::port() const { return ate_interaction->port(); }
