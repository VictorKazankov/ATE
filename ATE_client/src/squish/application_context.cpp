#include "squish/application_context.h"

#include "logger/logger.h"

using namespace squish;

void ApplicationContext::Attach(const std::shared_ptr<interaction::Interaction>& ate_interaction) {
  ate_interaction_ = ate_interaction;
}

bool ApplicationContext::IsRunning() const { return ate_interaction_ && ate_interaction_->IsConnectionOpened(); }

void ApplicationContext::Detach() {}

std::string ApplicationContext::SendCommand(const std::string& command) {
  if (!IsRunning()) {
    logger::error("[application context] ATE server wasn't connected. Please perform 'attachToApplication()' first");
    throw std::runtime_error("Connection to ATE server is not established");
  }

  return ate_interaction_->SendCommand(command);
}

std::string ApplicationContext::host() const { return ate_interaction_->host(); }

std::string ApplicationContext::port() const { return ate_interaction_->port(); }
