#include "squish/squish_API.h"

#include <cstdlib>

#include <boost/asio/io_context.hpp>

#include "ate_interaction.h"
#include "common.h"
#include "logger/logger.h"
#include "message_factory/message_factory.h"
#include "squish/application_context.h"
#include "utils/squish_types_converter.h"

using namespace squish;

namespace {
const std::string kBoardSection = "BOARD";
const std::string kAddressOption = "Address";
const std::string kPortOption = "Port";
const std::string kTestSettingSection = "TEST_SETTINGS";
const std::string kWaitForObjectTimeoutOption = "WaitForObjectTimeout";
constexpr auto kConfigEnvVar = "ATE_CLIENT_CONFIG";

int kDefaultWaitForObjectTimeoutInMs = 0;
const int kDefaultLongPressTimeoutInMs = 2000;
uint64_t kCorrelationId = 1;

static ApplicationContext& ApplicationContextInstance() {
  static ApplicationContext applicationContext;
  return applicationContext;
}

static uint64_t GetCorrelationId() { return kCorrelationId++; }

auto& GetMainIoContext() {
  static boost::asio::io_context io_context;
  return io_context;
}
}  // namespace

ApplicationContext& API::AttachToApplication(const std::string&) {
  logger::debug("ApplicationContext AttachToApplication()");
  if (ApplicationContextInstance().IsRunning()) {
    logger::warn("ate_interaction already exist");
    return ApplicationContextInstance();
  }

  const char* const config_from_env = std::getenv(kConfigEnvVar);
  const std::string config_file = config_from_env ? config_from_env : ATE_CLIENT_CONFIG_DIR "/ate_client.ini";
  logger::info("Config file: {}", config_file);
  common::SetUp(config_file);
  kDefaultWaitForObjectTimeoutInMs = common::Config().GetInt(kTestSettingSection, kWaitForObjectTimeoutOption, 0);

  auto host = common::Config().GetString(kBoardSection, kAddressOption, "");
  auto port = common::Config().GetString(kBoardSection, kPortOption, "");

  auto ate_interaction = std::make_unique<interaction::ATEInteraction>(GetMainIoContext(), host, port);
  ApplicationContextInstance().Attach(std::move(ate_interaction));

  return ApplicationContextInstance();
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
  return ApplicationContextInstance().SendCommand(interaction::Method::kWaitForObject, message);
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
  ApplicationContextInstance().SendCommand(interaction::Method::kTapObject, message);
}

void API::LongPress(const Object& screen_rectangle, int timeout_msec) {
  LongPress(screen_rectangle, 0, 0, timeout_msec);
}

void API::LongPress(const Object& screen_rectangle, int x, int y, int timeout_msec) {
  // Check validation of relative coordinates (should be less Object size)
  if (x > screen_rectangle.width || y > screen_rectangle.height || x < 0 || y < 0) {
    logger::error("LongPress: invalid coordinates [{},{}], must be less than [{},{}]", x, y, screen_rectangle.width,
                  screen_rectangle.height);
    return;
  }
  if (timeout_msec <= 0) {
    logger::warn("LongPress: invalid timeout {} milliseconds, use default {} milliseconds", timeout_msec,
                 kDefaultLongPressTimeoutInMs);
    timeout_msec = kDefaultLongPressTimeoutInMs;
  }

  std::string message;
  (!x && !y)
      ? message = common::jmsg::MessageFactory::Client::CreateLongPressRequest(
            screen_rectangle.Center().x, screen_rectangle.Center().y, static_cast<unsigned>(timeout_msec),
            GetCorrelationId())
      : message = common::jmsg::MessageFactory::Client::CreateLongPressRequest(
            screen_rectangle.x + x, screen_rectangle.y + y, static_cast<unsigned>(timeout_msec), GetCorrelationId());

  ApplicationContextInstance().SendCommand(interaction::Method::kLongPress, message);
}

void API::TouchAndDrag(const Object& object_or_name, int x, int y, int dx, int dy,
                       common::squish::ModifierState modifier_state) {
  TouchAndDrag(object_or_name.name, x, y, dx, dy, modifier_state);
}

void API::TouchAndDrag(const std::string& object_or_name, int x, int y, int dx, int dy,
                       common::squish::ModifierState modifier_state) {
  auto message = common::jmsg::MessageFactory::Client::CreateTouchAndDragRequest(object_or_name, x, y, dx, dy,
                                                                                 modifier_state, GetCorrelationId());
  ApplicationContextInstance().SendCommand(interaction::Method::kTouchAndDrag, message);
}

void API::PressAndHold(const common::Point& screen_point) {
  auto message = common::jmsg::MessageFactory::Client::CreatePressAndHoldRequest(screen_point.x, screen_point.y,
                                                                                 GetCorrelationId());
  ApplicationContextInstance().SendCommand(interaction::Method::kPressAndHold, message);
}

void API::PressAndHold(const common::Rect& screen_rectangle) { PressAndHold(screen_rectangle.Center()); }

void API::PressAndHold(const Object& object) { PressAndHold(object.Center()); }

void API::ChangeSyncIconDB(const std::string& sync_version, const std::string& sync_build_version) {
  auto message = common::jmsg::MessageFactory::Client::CreateChangeSyncIconDBRequest(sync_version, sync_build_version,
                                                                                     GetCorrelationId());

  ApplicationContextInstance().SendCommand(interaction::Method::kChangeSyncIconDB, message);
}

void API::ChangeSyncMode(common::squish::CollectionMode collection_mode) {
  auto message = common::jmsg::MessageFactory::Client::CreateChangeSyncModeRequest(CollectionModeToStr(collection_mode),
                                                                                   GetCorrelationId());
  ApplicationContextInstance().SendCommand(interaction::Method::kChangeSyncMode, message);
}
