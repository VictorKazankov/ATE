#include "interaction/ATE/tcp_session_handler.h"

#include <cassert>
#include <utility>

#include <jsoncpp/json/value.h>

#include "interaction/ATE/tcp_connection.h"
#include "logger/logger.h"
#include "message_factory/json_defines.h"
#include "message_factory/json_messages.h"
#include "message_factory/json_utils.h"
#include "message_factory/message_factory.h"
#include "rpc_error.h"
#include "utils/squish_types.h"

namespace interaction {

TcpSessionHandler::TcpSessionHandler(boost::asio::io_context& io_ctx)
    : ate_{io_ctx},
      handler_map_{{common::jmsg::kWaitForObject, &TcpSessionHandler::HandleWaitForObject},
                   {common::jmsg::kTapObject, &TcpSessionHandler::HandleTapObject}} {}

void TcpSessionHandler::OnOpen(TcpConnection& session) { session.Start(); }

void TcpSessionHandler::OnMessage(TcpConnection& session, const std::string& message) {
  std::uint64_t id = 0;
  std::string method;
  Json::Value params;
  std::pair<Json::Value, bool> result_or_error{Json::Value{Json::objectValue}, false};

  common::jmsg::ParseJsonRpcRequest(message, id, method, params, result_or_error.first);

  if (result_or_error.first.empty()) {
    // Successful parsing
    const auto method_handler = GetHandler(method);
    assert(method_handler);
    result_or_error = (this->*method_handler)(params);
  }

  SendResponse(session, id, std::move(result_or_error.first), result_or_error.second);
}

void TcpSessionHandler::SendResponse(TcpConnection& session, std::uint64_t id, Json::Value result_or_error,
                                     bool is_result) {
  std::string response =
      common::jmsg::MessageFactory::Server::CreateResponse(id, std::move(result_or_error), is_result);
  session.Send(response);
}

TcpSessionHandler::MessageHandlerFunction TcpSessionHandler::GetHandler(const std::string& method) const noexcept {
  try {
    return handler_map_.at(method);
  } catch (const std::out_of_range& /*not_found*/) {
    return &TcpSessionHandler::HandleUnknownMethod;
  }
}

std::pair<Json::Value, bool> TcpSessionHandler::HandleWaitForObject(const Json::Value& params) {
  std::string object_or_name;
  std::chrono::milliseconds timeout;
  Json::Value error;

  common::jmsg::ExtractWaitForObjectRequestParams(params, object_or_name, timeout, error);

  if (!error.empty()) {
    // Extract error occurs
    return std::make_pair(std::move(error), false);
  }

  const cv::Rect position = ate_.WaitForObject(object_or_name, timeout);

  if (position.empty()) {
    error = common::jmsg::CreateErrorObject(rpc::Error::kObjectNotFound, "Object not found");
    logger::info("[session_handler] object_or_name: {}, timeout: {}ms error: {}", object_or_name, timeout.count(),
                 error.toStyledString());
    return std::make_pair(std::move(error), false);
  }

  return std::make_pair(common::jmsg::MessageFactory::Server::CreateWaitForObjectResultObject(
                            position.x, position.y, position.width, position.height),
                        true);
}

std::pair<Json::Value, bool> TcpSessionHandler::HandleTapObject(const Json::Value& params) {
  cv::Point point;
  common::squish::ModifierState modifier_state = common::squish::ModifierState::NONE;
  common::squish::MouseButton mouse_button = common::squish::MouseButton::NONE;
  Json::Value extract_error;

  common::jmsg::ExtractTapObjectRequestParams(params, point.x, point.y, modifier_state, mouse_button, extract_error);

  if (!extract_error.empty()) {
    return std::make_pair(std::move(extract_error), false);
  }

  // TODO: we do not provide modifier state and mouse button in TapObject API
  ate_.TapObject(point);

  return std::make_pair(common::jmsg::MessageFactory::Server::CreateTapObjectResultObject(), true);
}

std::pair<Json::Value, bool> TcpSessionHandler::HandleUnknownMethod(const Json::Value& params) {
  Json::Value error = common::jmsg::CreateErrorObject(rpc::Error::kMethodNotFound, "Method not found");
  logger::error("[session_handler] {}\nparams: {}", error.toStyledString(), params.toStyledString());
  return std::make_pair(std::move(error), false);
}

}  // namespace interaction
