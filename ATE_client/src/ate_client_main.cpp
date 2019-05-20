#include "iostream"
#include "memory"

#include <boost/asio/io_context.hpp>
#include "logger/logger.h"

#include "ate_interaction.h"
#include "squish.h"

std::unique_ptr<interaction::ATEInteraction> ate_interaction;

namespace squish {
namespace API {

ApplicationContext WaitForApplicationLaunch(std::chrono::seconds /*timeout_secs*/) {
  std::clog << "waitForApplicationLaunch()\n";
  if (ate_interaction) {
    std::clog << "Warning ate_interaction already exist\n";
    return ApplicationContext();  // TODO implement
  }

  boost::asio::io_context io_context;

  try {
    ate_interaction = std::make_unique<interaction::ATEInteraction>(io_context, "127.0.0.1", "5000");
  } catch (const boost::system::system_error& boost_error) {
    logger::critical("boost system error: {} ({})", boost_error.what(), boost_error.code());
    exit(EXIT_FAILURE);
  } catch (const std::exception& exception) {
    logger::critical("fatal error: {}", exception.what());
    exit(EXIT_FAILURE);
  }

  io_context.run();

  return ApplicationContext();  // TODO implement
}

Object WaitForObject(const std::string& /*object_or_name*/) {
  std::clog << "WaitForObject()\n";
  return ate_interaction->SendCommand("dump command");  // TODO implement
}

Object WaitForObject(const std::string& /*object_or_name*/, std::chrono::milliseconds /*timeout_msec*/) {
  std::clog << "WaitForObject()\n";
  return ate_interaction->SendCommand("dump command");  // TODO implement
}

void TapObject(const ScreenPoint& /*screen_point*/) {
  std::clog << "TapObject()\n";
  ate_interaction->SendCommand("dump command");
  // TODO implement
}

void TapObject(const ScreenRectangle& /*screen_rectangle*/) {
  std::clog << "TapObject()\n";
  ate_interaction->SendCommand("dump command");
  // TODO implement
}

}  // namespace API
}  // namespace squish

int main() {
  std::cout << "ATE server main" << std::endl;

  squish::API::WaitForApplicationLaunch();

  return EXIT_SUCCESS;
}
