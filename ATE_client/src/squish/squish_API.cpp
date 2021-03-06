#include "squish/squish_API.h"

#include <cstdlib>

#include "logger/logger.h"
#include "message_factory/message_factory.h"

#include "error_defines.h"
#include "json_rpc_parser.h"

using namespace API;

void SquishApi::SetDefaultWaitForObjectTimeout(int timeout) { default_wait_for_object_timeout_in_ms_ = timeout; }

squish::ApplicationContext& SquishApi::AttachToApplication(
    const std::shared_ptr<interaction::Interaction>& ate_interaction, const std::string&) {
  logger::debug("ApplicationContext AttachToApplication()");
  if (application_context_.IsRunning()) {
    logger::warn("ate_interaction already exist");
    return application_context_;
  }

  application_context_.Attach(ate_interaction);

  return application_context_;
}

squish::Object SquishApi::WaitForObject(const std::shared_ptr<interaction::Interaction>& ate_interaction,
                                        const uint64_t& correlation_id, const squish::Object& object_or_name,
                                        const common::Point& top_left, const common::Point& bottom_right) const {
  return WaitForObject(ate_interaction, correlation_id, object_or_name, default_wait_for_object_timeout_in_ms_,
                       top_left, bottom_right);
}

squish::Object SquishApi::WaitForObject(const std::shared_ptr<interaction::Interaction>& ate_interaction,
                                        const uint64_t& correlation_id, const squish::Object& object_or_name,
                                        int timeout_msec, const common::Point& top_left,
                                        const common::Point& bottom_right) const {
  return WaitForObject(ate_interaction, correlation_id, object_or_name.name, timeout_msec, top_left, bottom_right);
}

squish::Object SquishApi::WaitForObject(const std::shared_ptr<interaction::Interaction>& ate_interaction,
                                        const uint64_t& correlation_id, const std::string& object_or_name,
                                        const common::Point& top_left, const common::Point& bottom_right) const {
  return WaitForObject(ate_interaction, correlation_id, object_or_name, default_wait_for_object_timeout_in_ms_,
                       top_left, bottom_right);
}

squish::Object SquishApi::WaitForObject(const std::shared_ptr<interaction::Interaction>& ate_interaction,
                                        const uint64_t& correlation_id, const std::string& object_or_name,
                                        int timeout_msec, const common::Point& top_left,
                                        const common::Point& bottom_right) const {
  logger::debug("Object waitForObject()");
  const auto message = common::jmsg::MessageFactory::Client::CreateWaitForObjectRequest(
      object_or_name, timeout_msec, top_left, bottom_right, correlation_id);
  const auto response = ate_interaction->SendCommand(message);
  return interaction::JsonRpcParser::ParseWaitForObject(response);
}

squish::Object SquishApi::WaitForObject(const std::shared_ptr<interaction::Interaction>& ate_interaction,
                                        const uint64_t& correlation_id, const squish::Wildcard& wildcard,
                                        const common::Point& top_left, const common::Point& bottom_right) const {
  return WaitForObject(ate_interaction, correlation_id, wildcard, default_wait_for_object_timeout_in_ms_, top_left,
                       bottom_right);
}

squish::Object SquishApi::WaitForObject(const std::shared_ptr<interaction::Interaction>& ate_interaction,
                                        const uint64_t& correlation_id, const squish::Wildcard& wildcard,
                                        int timeout_sec, const common::Point& top_left,
                                        const common::Point& bottom_right) const {
  logger::debug("Object wairForObject(Wildcard)");
  const auto message = common::jmsg::MessageFactory::Client::CreateWaitForObjectRequest(wildcard, timeout_sec, top_left,
                                                                                        bottom_right, correlation_id);
  const auto response = ate_interaction->SendCommand(message);
  return interaction::JsonRpcParser::ParseWaitForObject(response);
}

void SquishApi::TapObject(const std::shared_ptr<interaction::Interaction>& ate_interaction,
                          const uint64_t& correlation_id, const squish::Object& screen_rectangle,
                          common::squish::ModifierState modifier_state, common::squish::MouseButton button) const {
  TapObject(ate_interaction, correlation_id, screen_rectangle.Center(), modifier_state, button);
}

void SquishApi::TapObject(const std::shared_ptr<interaction::Interaction>& ate_interaction,
                          const uint64_t& correlation_id, const common::Rect& screen_rectangle,
                          common::squish::ModifierState modifier_state, common::squish::MouseButton button) const {
  TapObject(ate_interaction, correlation_id, screen_rectangle.Center(), modifier_state, button);
}

void SquishApi::TapObject(const std::shared_ptr<interaction::Interaction>& ate_interaction,
                          const uint64_t& correlation_id, const common::Point& screen_point,
                          common::squish::ModifierState modifier_state, common::squish::MouseButton button) const {
  logger::debug("Object tapObject");
  auto message = common::jmsg::MessageFactory::Client::CreateTapObjectRequest(screen_point.x, screen_point.y,
                                                                              modifier_state, button, correlation_id);
  const auto response = ate_interaction->SendCommand(message);
  interaction::JsonRpcParser::CheckAndRaiseExceptionInCaseErrors(response);
}

void SquishApi::LongPress(const std::shared_ptr<interaction::Interaction>& ate_interaction,
                          const uint64_t& correlation_id, const squish::Object& screen_rectangle,
                          int timeout_msec) const {
  LongPress(ate_interaction, correlation_id, screen_rectangle, 0, 0, timeout_msec);
}

void SquishApi::LongPress(const std::shared_ptr<interaction::Interaction>& ate_interaction,
                          const uint64_t& correlation_id, const squish::Object& screen_rectangle, int x, int y,
                          int timeout_msec) const {
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
}

void SquishApi::TouchAndDrag(const std::shared_ptr<interaction::Interaction>& ate_interaction,
                             const uint64_t& correlation_id, const squish::Object& object_or_name, int x, int y, int dx,
                             int dy, common::squish::ModifierState modifier_state) const {
  TouchAndDrag(ate_interaction, correlation_id, object_or_name.name, x, y, dx, dy, modifier_state);
}

void SquishApi::TouchAndDrag(const std::shared_ptr<interaction::Interaction>& ate_interaction,
                             const uint64_t& correlation_id, const std::string& object_or_name, int x, int y, int dx,
                             int dy, common::squish::ModifierState modifier_state) const {
  auto message = common::jmsg::MessageFactory::Client::CreateTouchAndDragRequest(object_or_name, x, y, dx, dy,
                                                                                 modifier_state, correlation_id);
  const auto response = ate_interaction->SendCommand(message);
  interaction::JsonRpcParser::CheckAndRaiseExceptionInCaseErrors(response);
}

void SquishApi::PressAndHold(const std::shared_ptr<interaction::Interaction>& ate_interaction,
                             const uint64_t& correlation_id, const common::Point& screen_point) const {
  auto message =
      common::jmsg::MessageFactory::Client::CreatePressAndHoldRequest(screen_point.x, screen_point.y, correlation_id);
  const auto response = ate_interaction->SendCommand(message);
  interaction::JsonRpcParser::CheckAndRaiseExceptionInCaseErrors(response);
}

void SquishApi::PressAndHold(const std::shared_ptr<interaction::Interaction>& ate_interaction,
                             const uint64_t& correlation_id, const common::Rect& screen_rectangle) const {
  PressAndHold(ate_interaction, correlation_id, screen_rectangle.Center());
}

void SquishApi::PressAndHold(const std::shared_ptr<interaction::Interaction>& ate_interaction,
                             const uint64_t& correlation_id, const squish::Object& object) const {
  PressAndHold(ate_interaction, correlation_id, object.Center());
}

void SquishApi::PressRelease(const std::shared_ptr<interaction::Interaction>& ate_interaction,
                             const uint64_t& correlation_id, const common::Point& screen_point) const {
  auto message =
      common::jmsg::MessageFactory::Client::CreatePressReleaseRequest(screen_point.x, screen_point.y, correlation_id);
  const auto response = ate_interaction->SendCommand(message);
  interaction::JsonRpcParser::CheckAndRaiseExceptionInCaseErrors(response);
}

void SquishApi::PressRelease(const std::shared_ptr<interaction::Interaction>& ate_interaction,
                             const uint64_t& correlation_id, const common::Rect& screen_rectangle) const {
  PressRelease(ate_interaction, correlation_id, screen_rectangle.Center());
}

void SquishApi::PressRelease(const std::shared_ptr<interaction::Interaction>& ate_interaction,
                             const uint64_t& correlation_id, const squish::Object& object) const {
  PressRelease(ate_interaction, correlation_id, object.Center());
}

bool SquishApi::Exists(const std::shared_ptr<interaction::Interaction>& ate_interaction, const uint64_t& correlation_id,
                       const std::string& object_name, const common::Point& top_left,
                       const common::Point& bottom_right) const {
  try {
    const int kOneSecondTimeout = 1;
    WaitForObject(ate_interaction, correlation_id, object_name, kOneSecondTimeout, top_left, bottom_right);
    return true;
  } catch (const squish::LookupError&) {
    return false;
  } catch (...) {
    throw;
  }
}

bool SquishApi::IsApplicationContextExist() const { return application_context_.IsRunning(); }
