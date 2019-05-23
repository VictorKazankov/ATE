#include "memory"

#include <boost/asio/io_context.hpp>
#include "logger/logger.h"

#include "ate_interaction.h"
#include "common.h"
#include "squish.h"

using namespace squish;

namespace {
const std::string kBoardSection = "BOARD";
const std::string kAddressOption = "Address";
const std::string kPortOption = "Port";
const std::string kTestSettingSection = "TEST_SETTINGS";
const std::string kWaitForObjectTimeoutOption = "WaitForObjectTimeout";

static int kDefaultWaitForObjectTimeoutInMs = 0;
}  // namespace

std::unique_ptr<interaction::ATEInteraction> ate_interaction;

ApplicationContext API::AttachToApplication(const std::string&) {
  logger::info("ApplicationContext AttachToApplication()");
  if (ate_interaction) {
    logger::warn("Warning ate_interaction already exist");
    return ApplicationContext();
  }

  boost::asio::io_context io_context;

  const std::string config_file{VHAT_CLIENT_DATA_PATH "/config.ini"};
  common::SetUp(config_file);
  kDefaultWaitForObjectTimeoutInMs = common::Config().GetInt(kTestSettingSection, kWaitForObjectTimeoutOption, 0);

  try {
    ate_interaction = std::make_unique<interaction::ATEInteraction>(
        io_context, common::Config().GetString(kBoardSection, kAddressOption, ""),
        common::Config().GetString(kBoardSection, kPortOption, ""));

  } catch (const boost::system::system_error& boost_error) {
    logger::critical("boost system error: {} ({})", boost_error.what(), boost_error.code());
    exit(EXIT_FAILURE);
  } catch (const std::exception& exception) {
    logger::critical("fatal error: {}", exception.what());
    exit(EXIT_FAILURE);
  }

  io_context.run();

  return ApplicationContext();
}

Object API::WaitForObject(const std::string& object_or_name) {
  return WaitForObject(object_or_name, kDefaultWaitForObjectTimeoutInMs);
}

Object API::WaitForObject(const Object& object_or_name) {
  return WaitForObject(object_or_name, kDefaultWaitForObjectTimeoutInMs);
}

Object API::WaitForObject(const squish::Object& /*object_or_name*/, int /*timeout_msec*/) {
  logger::info("Object WaitForObject()");
  return ate_interaction->SendCommand("dump command");  // TODO implement
}

Object API::WaitForObject(const std::string& /*object_or_name*/, int /*timeout_msec*/) {
  logger::info("Object WaitForObject()");
  return ate_interaction->SendCommand("dump command");  // TODO implement
}

void API::TapObject(const ScreenPoint& /*screen_point*/, ModifierState /*modifier_state*/, MouseButton /*button*/) {
  logger::info("void TapObject()");

  ate_interaction->SendCommand("dump command");  // TODO implement
}

void API::TapObject(const ScreenRectangle& screen_rectangle, ModifierState modifier_state, MouseButton button) {
  TapObject(screen_rectangle.Center(), modifier_state, button);
}

void API::TapObject(const Object& screen_rectangle, ModifierState modifier_state, MouseButton button) {
  TapObject(screen_rectangle.Center(), modifier_state, button);
}
