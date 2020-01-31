#include "squish/squish_API.h"

#include <cstdlib>

#include "logger/logger.h"
#include "message_factory/message_factory.h"
#include "utils/squish_types_converter.h"

#include "json_rpc_parser.h"

using namespace API;

namespace {
#define NO_RESPONSE_FOR_PYTHON logger::debug("No response for Python from {}", __func__)
}  // namespace

int SquishApi::default_wait_for_object_timeout_in_ms_ = 0;

void SquishApi::SetDefaultWaitForObjectTimeout(int timeout) {
  // TODO DEBUG
  logger::debug("default_wait_for_object_timeout_in_ms_ value is {}",
                default_wait_for_object_timeout_in_ms_);  // TODO REMOVE
  default_wait_for_object_timeout_in_ms_ = timeout;
  logger::debug("default_wait_for_object_timeout_in_ms_ value is {}",
                default_wait_for_object_timeout_in_ms_);  // TODO REMOVE
}

squish::ApplicationContext& SquishApi::AttachToApplication(
    const std::shared_ptr<interaction::ATEInteraction>& ate_interaction, const std::string&) {
  logger::debug("ApplicationContext AttachToApplication()");
  if (application_context_.IsRunning()) {
    logger::warn("ate_interaction already exist");
    return application_context_;
  }

  application_context_.Attach(ate_interaction);

  return application_context_;
}

squish::Object SquishApi::WaitForObject(const std::shared_ptr<interaction::ATEInteraction>& ate_interaction,
                                        const uint64_t& correlation_id, const squish::Object& object_or_name) {
  return WaitForObject(ate_interaction, correlation_id, object_or_name, default_wait_for_object_timeout_in_ms_);
}

squish::Object SquishApi::WaitForObject(const std::shared_ptr<interaction::ATEInteraction>& ate_interaction,
                                        const uint64_t& correlation_id, const squish::Object& object_or_name,
                                        int timeout_msec) {
  return WaitForObject(ate_interaction, correlation_id, object_or_name.name, timeout_msec);
}

squish::Object SquishApi::WaitForObject(const std::shared_ptr<interaction::ATEInteraction>& ate_interaction,
                                        const uint64_t& correlation_id, const std::string& object_or_name) {
  return WaitForObject(ate_interaction, correlation_id, object_or_name, default_wait_for_object_timeout_in_ms_);
}

squish::Object SquishApi::WaitForObject(const std::shared_ptr<interaction::ATEInteraction>& ate_interaction,
                                        const uint64_t& correlation_id, const std::string& object_or_name,
                                        int timeout_msec) {
  logger::debug("Object waitForObject()");
  const auto message =
      common::jmsg::MessageFactory::Client::CreateWaitForObjectRequest(object_or_name, timeout_msec, correlation_id);
  const auto response = ate_interaction->SendCommand(message);
  return interaction::JsonRpcParser::ParseWaitForObject(response);
}

void SquishApi::TapObject(const std::shared_ptr<interaction::ATEInteraction>& ate_interaction,
                          const uint64_t& correlation_id, const squish::Object& screen_rectangle,
                          common::squish::ModifierState modifier_state, common::squish::MouseButton button) {
  TapObject(ate_interaction, correlation_id, screen_rectangle.Center(), modifier_state, button);
}

void SquishApi::TapObject(const std::shared_ptr<interaction::ATEInteraction>& ate_interaction,
                          const uint64_t& correlation_id, const common::Rect& screen_rectangle,
                          common::squish::ModifierState modifier_state, common::squish::MouseButton button) {
  TapObject(ate_interaction, correlation_id, screen_rectangle.Center(), modifier_state, button);
}

void SquishApi::TapObject(const std::shared_ptr<interaction::ATEInteraction>& ate_interaction,
                          const uint64_t& correlation_id, const common::Point& screen_point,
                          common::squish::ModifierState modifier_state, common::squish::MouseButton button) {
  logger::debug("Object tapObject");
  auto message = common::jmsg::MessageFactory::Client::CreateTapObjectRequest(screen_point.x, screen_point.y,
                                                                              modifier_state, button, correlation_id);
  const auto response = ate_interaction->SendCommand(message);
  interaction::JsonRpcParser::CheckAndRaiseExceptionInCaseErrors(response);
  NO_RESPONSE_FOR_PYTHON;
}

void SquishApi::LongPress(const std::shared_ptr<interaction::ATEInteraction>& ate_interaction,
                          const uint64_t& correlation_id, const squish::Object& screen_rectangle, int timeout_msec) {
  LongPress(ate_interaction, correlation_id, screen_rectangle, 0, 0, timeout_msec);
}

void SquishApi::LongPress(const std::shared_ptr<interaction::ATEInteraction>& ate_interaction,
                          const uint64_t& correlation_id, const squish::Object& screen_rectangle, int x, int y,
                          int timeout_msec) {
  // Check validation of relative coordinates (should be less Object size)
  if (x > screen_rectangle.width || y > screen_rectangle.height || x < 0 || y < 0) {
    logger::error("LongPress: invalid coordinates [{},{}], must be less than [{},{}]", x, y, screen_rectangle.width,
                  screen_rectangle.height);
    return;
  }
  if (timeout_msec <= 0) {
    logger::warn("LongPress: invalid timeout {} milliseconds, use default {} milliseconds", timeout_msec,
                 default_long_press_timeout_in_ms_);
    timeout_msec = default_long_press_timeout_in_ms_;
  }

  std::string message;
  (!x && !y) ? message = common::jmsg::MessageFactory::Client::CreateLongPressRequest(
                   screen_rectangle.Center().x, screen_rectangle.Center().y, static_cast<unsigned>(timeout_msec),
                   correlation_id)
             : message = common::jmsg::MessageFactory::Client::CreateLongPressRequest(
                   screen_rectangle.x + x, screen_rectangle.y + y, static_cast<unsigned>(timeout_msec), correlation_id);

  const auto response = ate_interaction->SendCommand(message);
  interaction::JsonRpcParser::CheckAndRaiseExceptionInCaseErrors(response);
  NO_RESPONSE_FOR_PYTHON;
}

void SquishApi::TouchAndDrag(const std::shared_ptr<interaction::ATEInteraction>& ate_interaction,
                             const uint64_t& correlation_id, const squish::Object& object_or_name, int x, int y, int dx,
                             int dy, common::squish::ModifierState modifier_state) {
  TouchAndDrag(ate_interaction, correlation_id, object_or_name.name, x, y, dx, dy, modifier_state);
}

void SquishApi::TouchAndDrag(const std::shared_ptr<interaction::ATEInteraction>& ate_interaction,
                             const uint64_t& correlation_id, const std::string& object_or_name, int x, int y, int dx,
                             int dy, common::squish::ModifierState modifier_state) {
  auto message = common::jmsg::MessageFactory::Client::CreateTouchAndDragRequest(object_or_name, x, y, dx, dy,
                                                                                 modifier_state, correlation_id);
  const auto response = ate_interaction->SendCommand(message);
  interaction::JsonRpcParser::CheckAndRaiseExceptionInCaseErrors(response);
  NO_RESPONSE_FOR_PYTHON;
}

void SquishApi::PressAndHold(const std::shared_ptr<interaction::ATEInteraction>& ate_interaction,
                             const uint64_t& correlation_id, const common::Point& screen_point) {
  auto message =
      common::jmsg::MessageFactory::Client::CreatePressAndHoldRequest(screen_point.x, screen_point.y, correlation_id);
  const auto response = ate_interaction->SendCommand(message);
  interaction::JsonRpcParser::CheckAndRaiseExceptionInCaseErrors(response);
  NO_RESPONSE_FOR_PYTHON;
}

void SquishApi::PressAndHold(const std::shared_ptr<interaction::ATEInteraction>& ate_interaction,
                             const uint64_t& correlation_id, const common::Rect& screen_rectangle) {
  PressAndHold(ate_interaction, correlation_id, screen_rectangle.Center());
}

void SquishApi::PressAndHold(const std::shared_ptr<interaction::ATEInteraction>& ate_interaction,
                             const uint64_t& correlation_id, const squish::Object& object) {
  PressAndHold(ate_interaction, correlation_id, object.Center());
}

void SquishApi::PressRelease(const std::shared_ptr<interaction::ATEInteraction>& ate_interaction,
                             const uint64_t& correlation_id, const common::Point& screen_point) {
  auto message =
      common::jmsg::MessageFactory::Client::CreatePressReleaseRequest(screen_point.x, screen_point.y, correlation_id);
  const auto response = ate_interaction->SendCommand(message);
  interaction::JsonRpcParser::CheckAndRaiseExceptionInCaseErrors(response);
  NO_RESPONSE_FOR_PYTHON;
}

void SquishApi::PressRelease(const std::shared_ptr<interaction::ATEInteraction>& ate_interaction,
                             const uint64_t& correlation_id, const common::Rect& screen_rectangle) {
  PressRelease(ate_interaction, correlation_id, screen_rectangle.Center());
}

void SquishApi::PressRelease(const std::shared_ptr<interaction::ATEInteraction>& ate_interaction,
                             const uint64_t& correlation_id, const squish::Object& object) {
  PressRelease(ate_interaction, correlation_id, object.Center());
}

bool SquishApi::Exists(const std::shared_ptr<interaction::ATEInteraction>& ate_interaction,
                       const uint64_t& correlation_id, const std::string& object_name) {
  try {
    const int kOneSecondTimeout = 1;
    WaitForObject(ate_interaction, correlation_id, object_name, kOneSecondTimeout);
    return true;
  } catch (const std::runtime_error&) {
    return false;
  }
}

void SquishApi::ChangeSyncIconDB(const std::shared_ptr<interaction::ATEInteraction>& ate_interaction,
                                 const uint64_t& correlation_id, const std::string& sync_version,
                                 const std::string& sync_build_version) {
  auto message = common::jmsg::MessageFactory::Client::CreateChangeSyncIconDBRequest(sync_version, sync_build_version,
                                                                                     correlation_id);

  const auto response = ate_interaction->SendCommand(message);
  interaction::JsonRpcParser::CheckAndRaiseExceptionInCaseErrors(response);
  NO_RESPONSE_FOR_PYTHON;
}

void SquishApi::ChangeSyncMode(const std::shared_ptr<interaction::ATEInteraction>& ate_interaction,
                               const uint64_t& correlation_id, common::squish::CollectionMode collection_mode) {
  auto message = common::jmsg::MessageFactory::Client::CreateChangeSyncModeRequest(CollectionModeToStr(collection_mode),
                                                                                   correlation_id);
  const auto response = ate_interaction->SendCommand(message);
  interaction::JsonRpcParser::CheckAndRaiseExceptionInCaseErrors(response);
  NO_RESPONSE_FOR_PYTHON;
}

bool API::GetScreenshot(const std::shared_ptr<interaction::ATEInteraction>& ate_interaction,
                        const std::string& filename, const std::string& location) {
  const auto message =
      common::jmsg::MessageFactory::Client::CreateGetScreenshotRequest(filename, location, GetCorrelationId());
  const auto response = ate_interaction->SendCommand(message);

  return interaction::JsonRpcParser::ParseGetScreenshot(response);
}

std::string API::GetText(const std::shared_ptr<interaction::ATEInteraction>& ate_interaction, uint16_t x1, uint16_t y1,
                         uint16_t x2, uint16_t y2) {
  auto message = common::jmsg::MessageFactory::Client::CreateGetTextRequest(common::Point{x1, y1},
                                                                            common::Point{x2, y2}, GetCorrelationId());

  auto response = ate_interaction->SendCommand(message);
  return interaction::JsonRpcParser::ParseGetText(response);
}
