#include "ate_API.h"

#include <boost/asio/io_context.hpp>

#include "common.h"
#include "logger/logger.h"

//#include "squish/squish_API.h"

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

  // kDefaultWaitForObjectTimeoutInMs = common::Config().GetInt(kTestSettingSection, kWaitForObjectTimeoutOption, 0);

  auto host = common::Config().GetString(kBoardSection, kAddressOption, "");
  auto port = common::Config().GetString(kBoardSection, kPortOption, "");

  ate_interaction_ = std::make_shared<interaction::ATEInteraction>(GetMainIoContext(), host, port);
}

squish::ApplicationContext& AteApi::AttachToApplication(const std::string& /*aut_name*/) {
  // TODO
  squish::ApplicationContext* a = new squish::ApplicationContext();
  return *a;
}

squish::Object AteApi::WaitForObject(const std::string& /*object_or_name*/) {
  // TODO
  return squish::Object();
}

squish::Object AteApi::WaitForObject(const squish::Object& /*object_or_name*/) {
  // TODO
  return squish::Object();
}

squish::Object AteApi::WaitForObject(const std::string& /*object_or_name*/, int /*timeout_msec*/) {
  // TODO
  return squish::Object();
}

squish::Object AteApi::WaitForObject(const squish::Object& /*object_or_name*/, int /*timeout_msec*/) {
  // TODO
  return squish::Object();
}

void AteApi::TapObject(const common::Point& /*screen_point*/, common::squish::ModifierState /*modifier_state*/,
                       common::squish::MouseButton /*button*/) {
  // TODO
}

void AteApi::TapObject(const common::Rect& /*screen_rectangle*/, common::squish::ModifierState /*modifier_state*/,
                       common::squish::MouseButton /*button*/) {
  // TODO
}

void AteApi::TapObject(const squish::Object& /*screen_rectangle*/, common::squish::ModifierState /*modifier_state*/,
                       common::squish::MouseButton /*button*/) {
  // TODO
}

void AteApi::LongPress(const squish::Object& /*screen_rectangle*/, int /*timeout_msec*/) {
  // TODO
}

void AteApi::LongPress(const squish::Object& /*screen_rectangle*/, int /*x*/, int /*y*/, int /*timeout_msec*/) {
  // TODO
}

void AteApi::TouchAndDrag(const squish::Object& /*object_or_name*/, int /*x*/, int /*y*/, int /*dx*/, int /*dy*/,
                          common::squish::ModifierState /*modifier_state*/) {
  // TODO
}

void AteApi::TouchAndDrag(const std::string& /*object_or_name*/, int /*x*/, int /*y*/, int /*dx*/, int /*dy*/,
                          common::squish::ModifierState /*modifier_state*/) {
  // TODO
}

void AteApi::PressAndHold(const common::Point& /*screen_point*/) {
  // TODO
}

void AteApi::PressAndHold(const common::Rect& /*screen_rectangle*/) {
  // TODO
}

void AteApi::PressAndHold(const squish::Object& /*object*/) {
  // TODO
}

void AteApi::PressRelease(const common::Point& /*screen_point*/) {
  // TODO
}

void AteApi::PressRelease(const common::Rect& /*screen_rectangle*/) {
  // TODO
}

void AteApi::PressRelease(const squish::Object& /*object*/) {
  // TODO
}

void AteApi::ChangeSyncIconDB(const std::string& /*sync_version*/, const std::string& /*sync_build_version*/) {
  // TODO
}

void AteApi::ChangeSyncMode(common::squish::CollectionMode /*collection_mode*/) {
  // TODO
}