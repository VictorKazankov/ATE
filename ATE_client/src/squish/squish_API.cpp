#include "squish/squish_API.h"

#include <cstdlib>

#include <boost/asio/io_context.hpp>

#include "common.h"
#include "logger/logger.h"
#include "message_factory/message_factory.h"
#include "squish/application_context.h"

using namespace squish;

namespace {
const std::string kBoardSection = "BOARD";
const std::string kAddressOption = "Address";
const std::string kPortOption = "Port";
const std::string kTestSettingSection = "TEST_SETTINGS";
const std::string kWaitForObjectTimeoutOption = "WaitForObjectTimeout";
constexpr auto kConfigEnvVar = "VHAT_CLIENT_CONFIG";

int kDefaultWaitForObjectTimeoutInMs = 0;
uint64_t kCorrelationId = 1;

ApplicationContext applicationContext;

static uint64_t GetCorrelationId() { return kCorrelationId++; }

auto& GetMainIoContext() {
  static boost::asio::io_context io_context;
  return io_context;
}
}  // namespace

ApplicationContext API::AttachToApplication(const std::string&) {
  logger::debug("ApplicationContext AttachToApplication()");
  if (applicationContext.IsRunning()) {
    logger::warn("ate_interaction already exist");
    return applicationContext;
  }

  const char* const config_from_env = std::getenv(kConfigEnvVar);
  const std::string config_file = config_from_env ? config_from_env : VHAT_CLIENT_CONFIG_DIR "/vhat_client.ini";
  common::SetUp(config_file);
  logger::info("Config file: {}", config_file);
  kDefaultWaitForObjectTimeoutInMs = common::Config().GetInt(kTestSettingSection, kWaitForObjectTimeoutOption, 0);

  applicationContext.host = common::Config().GetString(kBoardSection, kAddressOption, "");
  applicationContext.port = common::Config().GetString(kBoardSection, kPortOption, "");

  applicationContext.Attach(GetMainIoContext());

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
  return applicationContext.SendCommand(interaction::Method::kWaitForObject, message);
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
  applicationContext.SendCommand(interaction::Method::kTapObject, message);
}

void API::TouchAndDrag(const Object& object_or_name, int x, int y, int dx, int dy) {
  TouchAndDrag(object_or_name.name, x, y, dx, dy, common::squish::ModifierState::NONE);
}

void API::TouchAndDrag(const std::string& object_or_name, int x, int y, int dx, int dy) {
  TouchAndDrag(object_or_name, x, y, dx, dy, common::squish::ModifierState::NONE);
}

void API::TouchAndDrag(const Object& object_or_name, int x, int y, int dx, int dy,
                       common::squish::ModifierState modifier_state) {
  TouchAndDrag(object_or_name.name, x, y, dx, dy, modifier_state);
}

void API::TouchAndDrag(const std::string& object_or_name, int x, int y, int dx, int dy,
                       common::squish::ModifierState modifier_state) {
  auto message = common::jmsg::MessageFactory::Client::CreateTouchAndDragRequest(object_or_name, x, y, dx, dy,
                                                                                 modifier_state, GetCorrelationId());
  applicationContext.SendCommand(interaction::Method::kTouchAndDrag, message);
}
