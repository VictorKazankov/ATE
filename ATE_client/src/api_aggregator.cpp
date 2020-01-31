#include "api_aggregator.h"

#include <boost/asio/io_context.hpp>

#include "common.h"
#include "logger/logger.h"

#include "squish/squish_API.h"

using namespace API;

namespace {
const std::string kBoardSection = "BOARD";
const std::string kAddressOption = "Address";
const std::string kPortOption = "Port";
const std::string kTestSettingSection = "TEST_SETTINGS";
const std::string kWaitForObjectTimeoutOption = "WaitForObjectTimeout";
constexpr auto kConfigEnvVar = "ATE_CLIENT_CONFIG";

auto& GetMainIoContext() {
  static boost::asio::io_context io_context;
  return io_context;
}
}  // namespace

ApiAggregator::ApiAggregator() {
  const char* const config_from_env = std::getenv(kConfigEnvVar);
  const std::string config_file = config_from_env ? config_from_env : ATE_CLIENT_CONFIG_DIR "/ate_client.ini";
  logger::info("Config file: {}", config_file);
  common::SetUp(config_file);

  SquishApi::SetDefaultWaitForObjectTimeout(
      common::Config().GetInt(kTestSettingSection, kWaitForObjectTimeoutOption, 0));

  auto host = common::Config().GetString(kBoardSection, kAddressOption, "");
  auto port = common::Config().GetString(kBoardSection, kPortOption, "");

  ate_interaction_ = std::make_shared<interaction::ATEInteraction>(GetMainIoContext(), host, port);
}

squish::ApplicationContext& ApiAggregator::AttachToApplication(const std::string& aut_name) {
  return SquishApi::AttachToApplication(ate_interaction_, aut_name);
}

squish::Object ApiAggregator::WaitForObject(const std::string& object_or_name) {
  return SquishApi::WaitForObject(ate_interaction_, object_or_name);
}

squish::Object ApiAggregator::WaitForObject(const squish::Object& object_or_name) {
  return SquishApi::WaitForObject(ate_interaction_, object_or_name);
}

squish::Object ApiAggregator::WaitForObject(const std::string& object_or_name, int timeout_msec) {
  return SquishApi::WaitForObject(ate_interaction_, object_or_name, timeout_msec);
}

squish::Object ApiAggregator::WaitForObject(const squish::Object& object_or_name, int timeout_msec) {
  return SquishApi::WaitForObject(ate_interaction_, object_or_name, timeout_msec);
}

void ApiAggregator::TapObject(const common::Point& screen_point, common::squish::ModifierState modifier_state,
                       common::squish::MouseButton button) {
  SquishApi::TapObject(ate_interaction_, screen_point, modifier_state, button);
}

void ApiAggregator::TapObject(const common::Rect& screen_rectangle, common::squish::ModifierState modifier_state,
                       common::squish::MouseButton button) {
  SquishApi::TapObject(ate_interaction_, screen_rectangle, modifier_state, button);
}

void ApiAggregator::TapObject(const squish::Object& screen_rectangle, common::squish::ModifierState modifier_state,
                       common::squish::MouseButton button) {
  SquishApi::TapObject(ate_interaction_, screen_rectangle, modifier_state, button);
}

void ApiAggregator::LongPress(const squish::Object& screen_rectangle, int timeout_msec) {
  SquishApi::LongPress(ate_interaction_, screen_rectangle, timeout_msec);
}

void ApiAggregator::LongPress(const squish::Object& screen_rectangle, int x, int y, int timeout_msec) {
  SquishApi::LongPress(ate_interaction_, screen_rectangle, x, y, timeout_msec);
}

void ApiAggregator::TouchAndDrag(const squish::Object& object_or_name, int x, int y, int dx, int dy,
                          common::squish::ModifierState modifier_state) {
  SquishApi::TouchAndDrag(ate_interaction_, object_or_name, x, y, dx, dy, modifier_state);
}

void ApiAggregator::TouchAndDrag(const std::string& object_or_name, int x, int y, int dx, int dy,
                          common::squish::ModifierState modifier_state) {
  SquishApi::TouchAndDrag(ate_interaction_, object_or_name, x, y, dx, dy, modifier_state);
}

void ApiAggregator::PressAndHold(const common::Point& screen_point) {
  SquishApi::PressAndHold(ate_interaction_, screen_point);
}

void ApiAggregator::PressAndHold(const common::Rect& screen_rectangle) {
  SquishApi::PressAndHold(ate_interaction_, screen_rectangle);
}

void ApiAggregator::PressAndHold(const squish::Object& object) { SquishApi::PressAndHold(ate_interaction_, object); }

void ApiAggregator::PressRelease(const common::Point& screen_point) {
  SquishApi::PressRelease(ate_interaction_, screen_point);
}

void ApiAggregator::PressRelease(const common::Rect& screen_rectangle) {
  SquishApi::PressRelease(ate_interaction_, screen_rectangle);
}

void ApiAggregator::PressRelease(const squish::Object& object) { SquishApi::PressRelease(ate_interaction_, object); }

bool ApiAggregator::Exists(const std::string& object_name) { return SquishApi::Exists(ate_interaction_, object_name); }

void ApiAggregator::ChangeSyncIconDB(const std::string& sync_version, const std::string& sync_build_version) {
  SquishApi::ChangeSyncIconDB(ate_interaction_, sync_version, sync_build_version);
}

void ApiAggregator::ChangeSyncMode(common::squish::CollectionMode collection_mode) {
  SquishApi::ChangeSyncMode(ate_interaction_, collection_mode);
}