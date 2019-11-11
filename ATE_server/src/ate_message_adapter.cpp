#include "ate_message_adapter.h"

#include <cassert>
#include <utility>

#include "json/value.h"

#include "ate_error.h"
#include "logger/logger.h"
#include "message_factory/json_defines.h"
#include "message_factory/json_messages.h"
#include "message_factory/json_utils.h"
#include "message_factory/message_factory.h"
#include "rpc_error.h"
#include "utils/squish_types.h"

AteMessageAdapter::AteMessageAdapter(ATE& ate)
    : ate_(ate),
      handler_map_{{common::jmsg::kWaitForObject, &AteMessageAdapter::HandleWaitForObject},
                   {common::jmsg::kTapObject, &AteMessageAdapter::HandleTapObject},
                   {common::jmsg::kTouchAndDrag, &AteMessageAdapter::HandleTouchAndDrag},
                   {common::jmsg::kDisplayTypeChanged, &AteMessageAdapter::HandleDisplayTypeChanged},
                   {common::jmsg::kChangeSyncIconDB, &AteMessageAdapter::HandleChangeSyncIconDB},
                   {common::jmsg::kChangeSyncMode, &AteMessageAdapter::HandleChangeSyncMode},
                   {common::jmsg::kReloadIconStorage, &AteMessageAdapter::HandleReloadIconStorage}} {}

std::string AteMessageAdapter::OnMessage(const std::string& message) {
  std::lock_guard<std::mutex> lock(on_message_guard_);

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
  return common::jmsg::MessageFactory::Server::CreateResponse(id, std::move(result_or_error.first),
                                                              result_or_error.second);
}

AteMessageAdapter::MessageHandlerFunction AteMessageAdapter::GetHandler(const std::string& method) const noexcept {
  try {
    return handler_map_.at(method);
  } catch (const std::out_of_range& /*not_found*/) {
    return &AteMessageAdapter::HandleUnknownMethod;
  }
}

std::pair<Json::Value, bool> AteMessageAdapter::HandleWaitForObject(const Json::Value& params) {
  std::string object_or_name;
  std::chrono::milliseconds timeout;
  Json::Value error;

  common::jmsg::ExtractWaitForObjectRequestParams(params, object_or_name, timeout, error);

  if (!error.empty()) {
    // Extract error occurs
    return std::make_pair(std::move(error), false);
  }

  cv::Rect position;
  std::error_code error_code;
  std::tie(position, error_code) = ate_.WaitForObject(object_or_name, timeout);

  if (error_code == common::AteError::kVideoTemporarilyUnavailable) {
    error = common::jmsg::CreateErrorObject(rpc::Error::kVideoStreamNotFound, error_code.message().c_str());
  } else if (error_code == common::AteError::kPatternInvalid || error_code == common::AteError::kPatternNotFound) {
    error = common::jmsg::CreateErrorObject(rpc::Error::kObjectNotFound, error_code.message().c_str());
  } else {
    logger::warn("[ate message adapter] unhandled error: {}, treated as object not found error");
    error = common::jmsg::CreateErrorObject(rpc::Error::kObjectNotFound, error_code.message().c_str());
  }

  if (!error.empty()) {
    logger::info("[ate message adapter] object_or_name: {}, timeout: {}ms error: {}", object_or_name, timeout.count(),
                 error.toStyledString());
    return std::make_pair(std::move(error), false);
  }

  return std::make_pair(common::jmsg::MessageFactory::Server::CreateWaitForObjectResultObject(
                            position.x, position.y, position.width, position.height),
                        true);
}

std::pair<Json::Value, bool> AteMessageAdapter::HandleTapObject(const Json::Value& params) {
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

std::pair<Json::Value, bool> AteMessageAdapter::HandleTouchAndDrag(const Json::Value& params) {
  std::string object_or_name;
  cv::Point start_point;
  cv::Point delta_point;
  common::squish::ModifierState modifier_state = common::squish::ModifierState::NONE;
  Json::Value extract_error;

  common::jmsg::ExtractTouchAndDragRequestParams(params, object_or_name, start_point.x, start_point.y, delta_point.x,
                                                 delta_point.y, modifier_state, extract_error);

  if (!extract_error.empty()) {
    return std::make_pair(std::move(extract_error), false);
  }

  ate_.TouchAndDrag(object_or_name, start_point, delta_point);

  return std::make_pair(common::jmsg::MessageFactory::Server::CreateTouchAndDragResultObject(), true);
}

std::pair<Json::Value, bool> AteMessageAdapter::HandleDisplayTypeChanged(const Json::Value& params) {
  int x;
  int y;
  Json::Value extract_error;

  common::jmsg::ExtractDisplayTypeChangedRequestParams(params, x, y, extract_error);

  if (!extract_error.empty()) {
    return std::make_pair(std::move(extract_error), false);
  }

  ate_.ChangeResolution(x, y);
  logger::info("[ate message adapter] Change resolution x:{} y:{}", x, y);

  return std::make_pair(common::jmsg::MessageFactory::DBusConnection::CreateDisplayTypeChangedResponse(), true);
}

std::pair<Json::Value, bool> AteMessageAdapter::HandleChangeSyncIconDB(const Json::Value& params) {
  std::string sync_version;
  std::string sync_build_version;
  Json::Value error;

  common::jmsg::ExtractChangeSyncIconDBRequestParams(params, sync_version, sync_build_version, error);
  if (!error.empty()) {
    return std::make_pair(std::move(error), false);
  }

  // Change configuration
  auto change_sync_info_error = ate_.ChangeSyncVersion(sync_version, sync_build_version);
  error = CheckChangeSyncConfigurationResult(change_sync_info_error);

  if (!error.empty()) {
    return std::make_pair(std::move(error), false);
  }

  return std::make_pair(common::jmsg::MessageFactory::Server::CreateChangeSyncIconDBResultObject(), true);
}

std::pair<Json::Value, bool> AteMessageAdapter::HandleChangeSyncMode(const Json::Value& params) {
  Json::Value error;
  std::string collection_mode;

  common::jmsg::ExtractChangeSyncModeRequestParams(params, collection_mode, error);

  if (!error.empty()) {
    return std::make_pair(std::move(error), false);
  }

  // Change Collection mode
  auto change_mode_error = ate_.ChangeSyncMode(collection_mode);
  error = CheckChangeSyncConfigurationResult(change_mode_error);

  if (!error.empty()) {
    return std::make_pair(std::move(error), false);
  }

  return std::make_pair(common::jmsg::MessageFactory::Server::CreateChangeSyncModeResultObject(), true);
}

std::pair<Json::Value, bool> AteMessageAdapter::HandleReloadIconStorage([[gnu::unused]] const Json::Value& params) {
  Json::Value result_error;

  std::error_code error = ate_.ReloadStorageItems();
  if (error) {
    result_error = common::jmsg::CreateErrorObject(rpc::Error::kReloadStorageError, error.message().data());
    logger::info("[ate message adapter] Error on reloading icon storage: {}", error.message());
  }

  logger::info("[ate message adapter] Icon storage has been reloaded");
  return std::make_pair(result_error, !error);
}

std::pair<Json::Value, bool> AteMessageAdapter::HandleUnknownMethod(const Json::Value& params) {
  Json::Value error = common::jmsg::CreateErrorObject(rpc::Error::kMethodNotFound, "Method not found");
  logger::error("[ate message adadpter] {}\nparams: {}", error.toStyledString(), params.toStyledString());
  return std::make_pair(std::move(error), false);
}

Json::Value AteMessageAdapter::CheckChangeSyncConfigurationResult(const adapter::DBManagerError& error) {
  switch (error) {
    case adapter::DBManagerError::kInvalidCollectionMode: {
      return common::jmsg::CreateErrorObject(rpc::Error::kInvalidSyncCollectionMode, "Invalid collection mode");
    }
    case adapter::DBManagerError::kInvalidSyncVersion: {
      return common::jmsg::CreateErrorObject(rpc::Error::kInvalidSyncVersion, "Invalid sync version");
    }
    case adapter::DBManagerError::kInvalidSyncBuildVersion: {
      return common::jmsg::CreateErrorObject(rpc::Error::kInvalidSyncBuildVersion, "Invalid sync build version");
    }
    case adapter::DBManagerError::kSuccess: {
      break;
    }
  }

  return {};
}
