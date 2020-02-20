#include "api_aggregator.h"

#include <boost/asio/io_context.hpp>

#include "common.h"
#include "logger/logger.h"

#include "error_defines.h"

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

uint64_t ApiAggregator::GetCorrelationId() { return correlation_id_++; }

void ApiAggregator::ThrowExceptionIfNoConnectionEstablished() const {
  if (!ate_interaction_) throw interaction::NoConnectionEstablished();
}

ApiAggregator::ApiAggregator() {
  const char* const config_from_env = std::getenv(kConfigEnvVar);
  const std::string config_file = config_from_env ? config_from_env : ATE_CLIENT_CONFIG_DIR "/ate_client.ini";
  logger::info("Config file: {}", config_file);
  common::SetUp(config_file);

  squish_api_.SetDefaultWaitForObjectTimeout(
      common::Config().GetInt(kTestSettingSection, kWaitForObjectTimeoutOption, 0));
}

squish::ApplicationContext& ApiAggregator::AttachToApplication(const std::string& aut_name) {
  if (!squish_api_.IsApplicationContextExist()) {
    auto host = common::Config().GetString(kBoardSection, kAddressOption, "");
    auto port = common::Config().GetString(kBoardSection, kPortOption, "");

    ate_interaction_ = std::make_shared<interaction::ATEInteraction>(GetMainIoContext(), host, port);
  }

  return squish_api_.AttachToApplication(ate_interaction_, aut_name);
}

squish::Object ApiAggregator::WaitForObject(const std::string& object_or_name) {
  ThrowExceptionIfNoConnectionEstablished();
  return squish_api_.WaitForObject(ate_interaction_, GetCorrelationId(), object_or_name);
}

squish::Object ApiAggregator::WaitForObject(const squish::Object& object_or_name) {
  ThrowExceptionIfNoConnectionEstablished();
  return squish_api_.WaitForObject(ate_interaction_, GetCorrelationId(), object_or_name);
}

squish::Object ApiAggregator::WaitForObject(const std::string& object_or_name, int timeout_msec) {
  ThrowExceptionIfNoConnectionEstablished();
  return squish_api_.WaitForObject(ate_interaction_, GetCorrelationId(), object_or_name, timeout_msec);
}

squish::Object ApiAggregator::WaitForObject(const squish::Object& object_or_name, int timeout_msec) {
  ThrowExceptionIfNoConnectionEstablished();
  return squish_api_.WaitForObject(ate_interaction_, GetCorrelationId(), object_or_name, timeout_msec);
}

void ApiAggregator::TapObject(const common::Point& screen_point, common::squish::ModifierState modifier_state,
                              common::squish::MouseButton button) {
  ThrowExceptionIfNoConnectionEstablished();
  squish_api_.TapObject(ate_interaction_, GetCorrelationId(), screen_point, modifier_state, button);
}

void ApiAggregator::TapObject(const common::Rect& screen_rectangle, common::squish::ModifierState modifier_state,
                              common::squish::MouseButton button) {
  ThrowExceptionIfNoConnectionEstablished();
  squish_api_.TapObject(ate_interaction_, GetCorrelationId(), screen_rectangle, modifier_state, button);
}

void ApiAggregator::TapObject(const squish::Object& screen_rectangle, common::squish::ModifierState modifier_state,
                              common::squish::MouseButton button) {
  ThrowExceptionIfNoConnectionEstablished();
  squish_api_.TapObject(ate_interaction_, GetCorrelationId(), screen_rectangle, modifier_state, button);
}

void ApiAggregator::LongPress(const squish::Object& screen_rectangle, int timeout_msec) {
  ThrowExceptionIfNoConnectionEstablished();
  squish_api_.LongPress(ate_interaction_, GetCorrelationId(), screen_rectangle, timeout_msec);
}

void ApiAggregator::LongPress(const squish::Object& screen_rectangle, int x, int y, int timeout_msec) {
  ThrowExceptionIfNoConnectionEstablished();
  squish_api_.LongPress(ate_interaction_, GetCorrelationId(), screen_rectangle, x, y, timeout_msec);
}

void ApiAggregator::TouchAndDrag(const squish::Object& object_or_name, int x, int y, int dx, int dy,
                                 common::squish::ModifierState modifier_state) {
  ThrowExceptionIfNoConnectionEstablished();
  squish_api_.TouchAndDrag(ate_interaction_, GetCorrelationId(), object_or_name, x, y, dx, dy, modifier_state);
}

void ApiAggregator::TouchAndDrag(const std::string& object_or_name, int x, int y, int dx, int dy,
                                 common::squish::ModifierState modifier_state) {
  ThrowExceptionIfNoConnectionEstablished();
  squish_api_.TouchAndDrag(ate_interaction_, GetCorrelationId(), object_or_name, x, y, dx, dy, modifier_state);
}

void ApiAggregator::PressAndHold(const common::Point& screen_point) {
  ThrowExceptionIfNoConnectionEstablished();
  squish_api_.PressAndHold(ate_interaction_, GetCorrelationId(), screen_point);
}

void ApiAggregator::PressAndHold(const common::Rect& screen_rectangle) {
  ThrowExceptionIfNoConnectionEstablished();
  squish_api_.PressAndHold(ate_interaction_, GetCorrelationId(), screen_rectangle);
}

void ApiAggregator::PressAndHold(const squish::Object& object) {
  ThrowExceptionIfNoConnectionEstablished();
  squish_api_.PressAndHold(ate_interaction_, GetCorrelationId(), object);
}

void ApiAggregator::PressRelease(const common::Point& screen_point) {
  ThrowExceptionIfNoConnectionEstablished();
  squish_api_.PressRelease(ate_interaction_, GetCorrelationId(), screen_point);
}

void ApiAggregator::PressRelease(const common::Rect& screen_rectangle) {
  ThrowExceptionIfNoConnectionEstablished();
  squish_api_.PressRelease(ate_interaction_, GetCorrelationId(), screen_rectangle);
}

void ApiAggregator::PressRelease(const squish::Object& object) {
  ThrowExceptionIfNoConnectionEstablished();
  squish_api_.PressRelease(ate_interaction_, GetCorrelationId(), object);
}

bool ApiAggregator::Exists(const std::string& object_name) {
  ThrowExceptionIfNoConnectionEstablished();
  return squish_api_.Exists(ate_interaction_, GetCorrelationId(), object_name);
}

void ApiAggregator::ChangeSyncIconDB(const std::string& sync_version, const std::string& sync_build_version) {
  ThrowExceptionIfNoConnectionEstablished();
  ate_api_.ChangeSyncIconDB(ate_interaction_, GetCorrelationId(), sync_version, sync_build_version);
}

void ApiAggregator::ChangeSyncMode(common::squish::CollectionMode collection_mode) {
  ThrowExceptionIfNoConnectionEstablished();
  ate_api_.ChangeSyncMode(ate_interaction_, GetCorrelationId(), collection_mode);
}

bool ApiAggregator::GetScreenshot(const std::string& filename, const std::string& location) {
  ThrowExceptionIfNoConnectionEstablished();
  return ate_api_.GetScreenshot(ate_interaction_, GetCorrelationId(), filename, location);
}

std::string ApiAggregator::GetText(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
  ThrowExceptionIfNoConnectionEstablished();
  return ate_api_.GetText(ate_interaction_, GetCorrelationId(), x1, y1, x2, y2);
}

std::vector<squish::Object> ApiAggregator::GetObjectsDataByPattern(const std::string& object_name) {
  ThrowExceptionIfNoConnectionEstablished();
  return ate_api_.GetObjectsDataByPattern(ate_interaction_, GetCorrelationId(), object_name);
}

std::vector<squish::Object> ApiAggregator::GetObjectsDataByPattern(const squish::Object& object_pattern) {
  ThrowExceptionIfNoConnectionEstablished();
  return ate_api_.GetObjectsDataByPattern(ate_interaction_, GetCorrelationId(), object_pattern);
}
