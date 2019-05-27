#include "interaction/ATE/tcp_session_handler.h"

#include <iostream>

#include "logger/logger.h"
#include "message_factory/json_defines.h"
#include "message_factory/json_messages.h"
#include "message_factory/json_utils.h"
#include "message_factory/message_factory.h"

namespace interaction {

namespace {

bool CheckMessage(const std::string& message, Json::Value& value) {
  if (!common::jmsg::ParseJson(message, value)) {
    return false;
  }
  if (!common::jmsg::CheckHeader(value)) {
    return false;
  }
  if (!common::jmsg::CheckHeaderType(value)) {
    return false;
  }
  return true;
}

}  // namespace

TcpSessionHandler::TcpSessionHandler()
    : handler_map_{{common::jmsg::kAttachToApplication,
                    [this](const std::shared_ptr<TcpConnection>& session, const Json::Value& json_message) {
                      this->HandleAttachToApplication(session, json_message);
                    }},
                   {common::jmsg::kWaitForObject,
                    [this](const std::shared_ptr<TcpConnection>& session, const Json::Value& json_message) {
                      this->HandleWaitForObject(session, json_message);
                    }},
                   {common::jmsg::kTapObject,
                    [this](const std::shared_ptr<TcpConnection>& session, const Json::Value& json_message) {
                      this->HandleTapObject(session, json_message);
                    }}} {}

void TcpSessionHandler::OnOpen(const std::shared_ptr<TcpConnection>& session) { session->Start(); }

void TcpSessionHandler::OnMessage(const std::shared_ptr<TcpConnection>& session, const std::string& message) {
  logger::info("[sessionhandler] receive message : {}", message);
  Json::Value json_message;
  if (!CheckMessage(message, json_message)) return;

  const auto& command = json_message[common::jmsg::kMethod].asString();
  const auto& it = handler_map_.find(command);

  if (it == handler_map_.end()) {
    logger::error("[sessionhandler] Unexpected command type");
    return;
  }
  it->second(session, json_message);
}

void TcpSessionHandler::HandleAttachToApplication(const std::shared_ptr<TcpConnection>& session,
                                                  const Json::Value& json_message) {
  if (!common::jmsg::CheckAttachToApplicationRequest(json_message)) return;
  logger::debug("Processing on WaitForApplicationLaunch");

  // avoid unused variable
  (void)(session);
  // Exctract: timout
  // TODO : ate.AttachToApplication(timeout);
}

void TcpSessionHandler::HandleWaitForObject(const std::shared_ptr<TcpConnection>& session,
                                            const Json::Value& json_message) {
  if (!common::jmsg::CheckWaitForObjectRequest(json_message)) return;
  logger::debug("Processing on WaitForObject");

  // avoid unused variable
  (void)(session);
  // Extract: object, timeout
  // TODO: ate.WaitForObject(object, timeout);
}

void TcpSessionHandler::HandleTapObject(const std::shared_ptr<TcpConnection>& session,
                                        const Json::Value& json_message) {
  if (!common::jmsg::CheckTapObjectRequest(json_message)) return;
  logger::debug("Processing on TapObject");

  // avoid unused variable
  (void)(session);
  // Extract: x, y or x, y, width, height and construct Point or Rect
  // TODO: ate.TapObject(Rect/Point);
}

}  // namespace interaction
