#include "squish/application_context.h"

#include "logger/logger.h"

using namespace squish;

ApplicationContext::ApplicationContext(const squish::ApplicationContext& rhs) : host(rhs.host), port(rhs.port) {}

void ApplicationContext::Attach(boost::asio::io_context& io_context) {
  ate_interaction = std::make_unique<interaction::ATEInteraction>(io_context, host, port);
}

bool ApplicationContext::IsRunning() const {
  if (ate_interaction && ate_interaction->IsConnectionOpened()) return true;

  return false;
}

void ApplicationContext::Detach() {}

Object ApplicationContext::SendCommand(interaction::Method method, const std::string& command) {
  if (!IsRunning()) {
    logger::critical(
        "[application context] ATE server wasn't connected. Please perform 'attachToApplication()' first");
    return Object{};
  }
  return ate_interaction->SendCommand(method, command);
}
