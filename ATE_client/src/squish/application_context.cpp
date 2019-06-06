#include "squish/application_context.h"

using namespace squish;

ApplicationContext::ApplicationContext(const squish::ApplicationContext& rhs) : host(rhs.host), port(rhs.port) {}

void ApplicationContext::Attach(boost::asio::io_context& io_context) {
  ate_interaction = std::make_unique<interaction::ATEInteraction>(io_context, host, port);
}

bool ApplicationContext::IsRunning() const {
  if (ate_interaction) return true;

  return false;
}

void ApplicationContext::Detach() {}

Object ApplicationContext::SendCommand(const std::string& command) { return ate_interaction->SendCommand(command); }