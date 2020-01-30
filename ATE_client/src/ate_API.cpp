#include "ate_API.h"

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

AteApi::AteApi() {
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

squish::ApplicationContext& AteApi::AttachToApplication(const std::string& aut_name) {
  return SquishApi::AttachToApplication(ate_interaction_, aut_name);
}

squish::Object AteApi::WaitForObject(const std::string& object_or_name) {
  return SquishApi::WaitForObject(ate_interaction_, object_or_name);
}

squish::Object AteApi::WaitForObject(const squish::Object& object_or_name) {
  return SquishApi::WaitForObject(ate_interaction_, object_or_name);
}

squish::Object AteApi::WaitForObject(const std::string& object_or_name, int timeout_msec) {
  return SquishApi::WaitForObject(ate_interaction_, object_or_name, timeout_msec);
}

squish::Object AteApi::WaitForObject(const squish::Object& object_or_name, int timeout_msec) {
  return SquishApi::WaitForObject(ate_interaction_, object_or_name, timeout_msec);
}

void AteApi::TapObject(const common::Point& screen_point, common::squish::ModifierState modifier_state,
                       common::squish::MouseButton button) {
  SquishApi::TapObject(ate_interaction_, screen_point, modifier_state, button);
}

void AteApi::TapObject(const common::Rect& screen_rectangle, common::squish::ModifierState modifier_state,
                       common::squish::MouseButton button) {
  SquishApi::TapObject(ate_interaction_, screen_rectangle, modifier_state, button);
}

void AteApi::TapObject(const squish::Object& screen_rectangle, common::squish::ModifierState modifier_state,
                       common::squish::MouseButton button) {
  SquishApi::TapObject(ate_interaction_, screen_rectangle, modifier_state, button);
}

void AteApi::LongPress(const squish::Object& screen_rectangle, int timeout_msec) {
  SquishApi::LongPress(ate_interaction_, screen_rectangle, timeout_msec);
}

void AteApi::LongPress(const squish::Object& screen_rectangle, int x, int y, int timeout_msec) {
  SquishApi::LongPress(ate_interaction_, screen_rectangle, x, y, timeout_msec);
}

void AteApi::TouchAndDrag(const squish::Object& object_or_name, int x, int y, int dx, int dy,
                          common::squish::ModifierState modifier_state) {
  SquishApi::TouchAndDrag(ate_interaction_, object_or_name, x, y, dx, dy, modifier_state);
}

void AteApi::TouchAndDrag(const std::string& object_or_name, int x, int y, int dx, int dy,
                          common::squish::ModifierState modifier_state) {
  SquishApi::TouchAndDrag(ate_interaction_, object_or_name, x, y, dx, dy, modifier_state);
}

void AteApi::PressAndHold(const common::Point& screen_point) {
  SquishApi::PressAndHold(ate_interaction_, screen_point);
}

void AteApi::PressAndHold(const common::Rect& screen_rectangle) {
  SquishApi::PressAndHold(ate_interaction_, screen_rectangle);
}

void AteApi::PressAndHold(const squish::Object& object) { SquishApi::PressAndHold(ate_interaction_, object); }

void AteApi::PressRelease(const common::Point& screen_point) {
  SquishApi::PressRelease(ate_interaction_, screen_point);
}

void AteApi::PressRelease(const common::Rect& screen_rectangle) {
  SquishApi::PressRelease(ate_interaction_, screen_rectangle);
}

void AteApi::PressRelease(const squish::Object& object) { SquishApi::PressRelease(ate_interaction_, object); }

bool AteApi::Exists(const std::string& object_name) { return SquishApi::Exists(ate_interaction_, object_name); }

void AteApi::ChangeSyncIconDB(const std::string& sync_version, const std::string& sync_build_version) {
  SquishApi::ChangeSyncIconDB(ate_interaction_, sync_version, sync_build_version);
}

void AteApi::ChangeSyncMode(common::squish::CollectionMode collection_mode) {
  SquishApi::ChangeSyncMode(ate_interaction_, collection_mode);
}