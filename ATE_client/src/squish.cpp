#include "squish.h"

#include <memory>

#include <boost/asio/io_context.hpp>
#include "logger/logger.h"

#include "ate_interaction.h"
#include "common.h"
#include "message_factory/message_factory.h"

using namespace squish;

namespace {
const std::string kBoardSection = "BOARD";
const std::string kAddressOption = "Address";
const std::string kPortOption = "Port";
const std::string kTestSettingSection = "TEST_SETTINGS";
const std::string kWaitForObjectTimeoutOption = "WaitForObjectTimeout";

int kDefaultWaitForObjectTimeoutInMs = 0;
uint64_t kCorrelationId = 1;

std::unique_ptr<interaction::ATEInteraction> ate_interaction;
ApplicationContext applicationContext;

static uint64_t GetCorrelationId() { return kCorrelationId++; }
}  // namespace

ApplicationContext API::AttachToApplication(const std::string&) {
  logger::debug("ApplicationContext AttachToApplication()");
  if (ate_interaction) {
    logger::warn("ate_interaction already exist");
    return applicationContext;
  }

  boost::asio::io_context io_context;

  const std::string config_file{VHAT_CLIENT_DATA_PATH "/config.ini"};
  common::SetUp(config_file);
  kDefaultWaitForObjectTimeoutInMs = common::Config().GetInt(kTestSettingSection, kWaitForObjectTimeoutOption, 0);

  applicationContext.host = common::Config().GetString(kBoardSection, kAddressOption, "");
  applicationContext.port = common::Config().GetString(kBoardSection, kPortOption, "");

  ate_interaction =
      std::make_unique<interaction::ATEInteraction>(io_context, applicationContext.host, applicationContext.port);

  return applicationContext;
}

Object API::WaitForObject(const Object& object_or_name) {
  return WaitForObject(object_or_name, kDefaultWaitForObjectTimeoutInMs);
}

Object API::WaitForObject(const squish::Object& object_or_name, int timeout_msec) {
  return WaitForObject(object_or_name.name, timeout_msec);
}

Object API::WaitForObject(const std::string& object_or_name) {
  return WaitForObject(object_or_name, kDefaultWaitForObjectTimeoutInMs);
}

Object API::WaitForObject(const std::string& object_or_name, int timeout_msec) {
  logger::debug("Object waitForObject()");
  auto message = common::jmsg::MessageFactory::Client::CreateWaitForObjectRequest(object_or_name, timeout_msec,
                                                                                  GetCorrelationId());
  return ate_interaction->SendCommand(message);
}

void API::TapObject(const Object& screen_rectangle, common::squish::ModifierState modifier_state,
                    common::squish::MouseButton button) {
  TapObject(screen_rectangle.Center(), modifier_state, button);
}

void API::TapObject(const common::Rect& screen_rectangle, common::squish::ModifierState modifier_state,
                    common::squish::MouseButton button) {
  TapObject(screen_rectangle.Center(), modifier_state, button);
}

void API::TapObject(const common::Point& screen_point, common::squish::ModifierState modifier_state,
                    common::squish::MouseButton button) {
  logger::debug("Object tapObject");
  auto message = common::jmsg::MessageFactory::Client::CreateTapObjectRequest(
      screen_point.x, screen_point.y, modifier_state, button, GetCorrelationId());
  ate_interaction->SendCommand(message);
}
