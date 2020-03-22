#include "message_factory/message_factory.h"

#include <utility>

#include "json/writer.h"

#include "message_factory/json_defines.h"
#include "message_factory/json_messages.h"
#include "utils/squish_types_converter.h"

namespace common {
namespace jmsg {

namespace {

void CreatePackageStructure(Json::Value& structure, const std::string& method, uint64_t id) {
  structure[kJsonRpc] = kRpcVersion;
  structure[kMethod] = method;
  structure[kId] = id;
}

void PutObjectDataIdentityIntoRequest(const ObjectDataIdentity& object_data_identity, Json::Value& params) {
  params[kName] = object_data_identity.name;
  params[kSyncVersion] = object_data_identity.sync_version;
  params[kSyncBuildVersion] = object_data_identity.build_version;
  params[kParentScreen] = object_data_identity.parent_screen;
  params[kSyncCollectionMode] = squish::CollectionModeToStr(object_data_identity.mode);
}

}  // namespace

std::string MessageFactory::Client::CreateAttachToApplicationRequest(int timeout_msec, uint64_t id) {
  Json::Value message;
  Json::Value params;
  Json::FastWriter writer;

  params[kTimeoutMsec] = timeout_msec;

  CreatePackageStructure(message, kAttachToApplication, id);
  message[kParams] = params;

  return writer.write(message);
}

std::string MessageFactory::Client::CreateWaitForObjectRequest(const std::string& object_name, int timeout_msec,
                                                               uint64_t id) {
  Json::Value params;
  Json::FastWriter writer;

  params[kName] = object_name;
  params[kTimeoutMsec] = timeout_msec;

  Json::Value message;

  CreatePackageStructure(message, kWaitForObject, id);
  message[kParams] = params;

  return writer.write(message);
}

std::string MessageFactory::Client::CreateWaitForObjectRequest(const common::ObjectDataIdentity& object_data_identity,
                                                               int timeout_msec, uint64_t id) {
  Json::Value params;
  Json::FastWriter writer;

  PutObjectDataIdentityIntoRequest(object_data_identity, params);
  params[kTimeoutMsec] = timeout_msec;

  Json::Value message;
  CreatePackageStructure(message, kWaitForObject, id);
  message[kParams] = params;

  return writer.write(message);
}

std::string MessageFactory::Client::CreateTapObjectRequest(int x, int y, squish::ModifierState modifier_state,
                                                           squish::MouseButton mouse_button, uint64_t id) {
  Json::Value params;
  Json::FastWriter writer;

  params[kAbscissa] = x;
  params[kOrdinate] = y;
  params[kModifierState] = static_cast<int>(modifier_state);
  params[kMouseButton] = static_cast<int>(mouse_button);

  Json::Value message;

  CreatePackageStructure(message, kTapObject, id);
  message[kParams] = params;

  return writer.write(message);
}

std::string MessageFactory::Client::CreateTouchAndDragRequest(const std::string& object_or_name, int x, int y, int dx,
                                                              int dy, squish::ModifierState modifier_state,
                                                              uint64_t id) {
  Json::Value params;
  Json::FastWriter writer;

  params[kObjectName] = object_or_name;
  params[kAbscissa] = x;
  params[kOrdinate] = y;
  params[kAbscissaDrag] = dx;
  params[kOrdinateDrag] = dy;
  params[kModifierState] = static_cast<int>(modifier_state);

  Json::Value message;

  CreatePackageStructure(message, kTouchAndDrag, id);
  message[kParams] = params;

  return writer.write(message);
}

std::string MessageFactory::Client::CreatePressAndHoldRequest(int x, int y, uint64_t id) {
  Json::Value params;
  Json::FastWriter writer;

  params[kAbscissa] = x;
  params[kOrdinate] = y;

  Json::Value message;

  CreatePackageStructure(message, kPressAndHold, id);
  message[kParams] = params;

  return writer.write(message);
}

std::string MessageFactory::Client::CreatePressReleaseRequest(int x, int y, uint64_t id) {
  Json::Value params;
  Json::FastWriter writer;

  params[kAbscissa] = x;
  params[kOrdinate] = y;

  Json::Value message;

  CreatePackageStructure(message, kPressRelease, id);
  message[kParams] = params;

  return writer.write(message);
}

std::string MessageFactory::Client::CreateChangeSyncIconDBRequest(const std::string& sync_version,
                                                                  const std::string& sync_build_version, uint64_t id) {
  Json::Value params;
  Json::FastWriter writer;

  params[kSyncVersion] = sync_version;
  params[kSyncBuildVersion] = sync_build_version;

  Json::Value message;
  CreatePackageStructure(message, kChangeSyncIconDB, id);
  message[kParams] = params;

  return writer.write(message);
}

std::string MessageFactory::Client::CreateChangeSyncModeRequest(const std::string& collection_mode, uint64_t id) {
  Json::Value params;
  Json::FastWriter writer;

  params[kSyncCollectionMode] = collection_mode;

  Json::Value message;

  CreatePackageStructure(message, kChangeSyncMode, id);
  message[kParams] = params;

  return writer.write(message);
}

std::string MessageFactory::Client::CreateLongPressRequest(int x, int y, int timeout_msec, uint64_t id) {
  Json::Value params;
  Json::FastWriter writer;

  params[kAbscissa] = x;
  params[kOrdinate] = y;
  params[kTimeoutMsec] = timeout_msec;

  Json::Value message;

  CreatePackageStructure(message, kLongPress, id);
  message[kParams] = params;

  return writer.write(message);
}

std::string MessageFactory::Client::CreateGetScreenshotRequest(const std::string& filename, const std::string& location,
                                                               uint64_t id) {
  Json::Value params;
  Json::FastWriter writer;

  params[kFileName] = filename;
  params[kLocation] = location;

  Json::Value message;

  CreatePackageStructure(message, kGetScreenshot, id);
  message[kParams] = params;

  return writer.write(message);
}

std::string MessageFactory::Client::CreateGetTextRequest(const common::Point& top_left,
                                                         const common::Point& bottom_right, uint64_t id) {
  Json::Value params;
  Json::FastWriter writer;

  params[kAbscissa] = top_left.x;
  params[kOrdinate] = top_left.y;
  params[kAbscissaDrag] = bottom_right.x;
  params[kOrdinateDrag] = bottom_right.y;

  Json::Value message;

  CreatePackageStructure(message, kGetText, id);
  message[kParams] = params;

  return writer.write(message);
}

/**
 * A JSON request contains next data in data section:
 * [{name}]
 */
std::string MessageFactory::Client::CreateGetObjectsDataByPatternRequest(const std::string& object_name, uint64_t id) {
  Json::Value params;
  Json::FastWriter writer;

  params[kName] = object_name;

  Json::Value message;
  CreatePackageStructure(message, kGetObjectsDataByPattern, id);
  message[kParams] = params;

  return writer.write(message);
}

/**
 * A JSON request contains next data in data section:
 * [{object_data_identity}]
 */
std::string MessageFactory::Client::CreateGetObjectsDataByPatternRequest(const ObjectDataIdentity& object_data_identity,
                                                                         uint64_t id) {
  Json::Value params;
  Json::FastWriter writer;

  PutObjectDataIdentityIntoRequest(object_data_identity, params);

  Json::Value message;

  CreatePackageStructure(message, kGetObjectsDataByPattern, id);
  message[kParams] = params;

  return writer.write(message);
}

/**
 * A JSON request contains next data in data section:
 * [{icon_path_second, icon_path_first, x_top_left, y_top_left, x_bottom_right, y_bottom_right}]
 */
std::string MessageFactory::Client::CreateGetImagesDiscrepancyRequest(const std::string& icon_path_second,
                                                                      const std::string& icon_path_first,
                                                                      const common::Point& top_left_coordinate,
                                                                      const common::Point& bottom_right_coordinate,
                                                                      uint64_t id) {
  Json::Value params;
  Json::FastWriter writer;

  params[kIconPathSecond] = icon_path_second;
  params[kIconPathFirst] = icon_path_first;
  params[kXTopLeft] = top_left_coordinate.x;
  params[kYTopLeft] = top_left_coordinate.y;
  params[kXBottomRight] = bottom_right_coordinate.x;
  params[kYBottomRight] = bottom_right_coordinate.y;

  Json::Value message;

  CreatePackageStructure(message, kGetImagesDiscrepancy, id);
  message[kParams] = params;

  return writer.write(message);
}

/**
 * A JSON request contains next data in data section:
 * [{time_interval_msec, timeout_msec, x_top_left, y_top_left, x_bottom_right, y_bottom_right, location}]
 */
std::string MessageFactory::Client::CreateCaptureFramesRequest(int interval, int duration,
                                                               const common::Point& left_top,
                                                               const common::Point& bottom_right,
                                                               const std::string& path, uint64_t id) {
  Json::Value params;
  Json::FastWriter writer;
  params[kTimeInterval] = interval;
  params[kTimeoutMsec] = duration;
  params[kXTopLeft] = left_top.x;
  params[kYTopLeft] = left_top.y;
  params[kXBottomRight] = bottom_right.x;
  params[kYBottomRight] = bottom_right.y;
  params[kLocation] = path.empty() ? "." : path;

  Json::Value message;

  CreatePackageStructure(message, kCaptureFrames, id);
  message[kParams] = params;

  return writer.write(message);
}

std::string MessageFactory::Server::CreateResponse(std::uint64_t id, Json::Value result_or_error, bool is_result) {
  Json::Value response{Json::objectValue};

  response[kJsonRpc] = kRpcVersion;

  if (id) {
    response[kId] = static_cast<Json::UInt64>(id);
  } else {
    response[kId] = Json::nullValue;
  }

  response[is_result ? kResult : kError] = std::move(result_or_error);

  Json::FastWriter writer;
  return writer.write(response);
}

Json::Value MessageFactory::Server::CreateTapObjectResultObject() { return Json::Value{true}; }

Json::Value MessageFactory::Server::CreateTouchAndDragResultObject() { return Json::Value{true}; }

Json::Value MessageFactory::Server::CreatePressAndHoldResultObject() { return Json::Value{true}; }

Json::Value MessageFactory::Server::CreatePressReleaseResultObject() { return Json::Value{true}; }

Json::Value MessageFactory::Server::CreateGetScreenshotObject() { return Json::Value{true}; }

Json::Value MessageFactory::Server::CreateWaitForObjectResultObject(int x, int y, int width, int height) {
  Json::Value result{Json::objectValue};

  result[kAbscissa] = x;
  result[kOrdinate] = y;
  result[kWidth] = width;
  result[kHeight] = height;

  return std::move(result);
}

Json::Value MessageFactory::Server::CreateChangeSyncIconDBResultObject() { return Json::Value{true}; }

Json::Value MessageFactory::Server::CreateChangeSyncModeResultObject() { return Json::Value{true}; }

Json::Value MessageFactory::Server::CreateLongPressResultObject() { return Json::Value{true}; }

Json::Value MessageFactory::Server::CreateGetTextResultObject(const std::string& text) {
  Json::Value result{Json::objectValue};
  result[kText] = text;

  return result;
}

/**
 * A JSON response contains an array of next structures of objects in data section:
 * [{x, y, width, height, x_top_left, y_top_left, x_bottom_right, y_bottom_right, parent_width, parent_height, name,
 * kParentScreen}]
 */
Json::Value MessageFactory::Server::CreateGetObjectsDataByPatternResponse(const std::vector<ObjectData>& objects_data) {
  Json::Value result;

  for (const auto& data : objects_data) {
    Json::Value node;
    node[kAbscissa] = data.center.x;
    node[kOrdinate] = data.center.y;
    node[kWidth] = data.width;
    node[kHeight] = data.height;
    node[kXTopLeft] = data.top_left.x;
    node[kYTopLeft] = data.top_left.y;
    node[kXBottomRight] = data.bottom_right.x;
    node[kYBottomRight] = data.bottom_right.y;
    node[kParentWidth] = data.parent_width;
    node[kParentHeight] = data.parent_height;
    node[kName] = data.name;
    node[kParentScreen] = data.parent_screen;
    node[kSyncVersion] = data.sync_version;
    node[kSyncBuildVersion] = data.build_version;
    node[kSyncCollectionMode] = squish::CollectionModeToStr(data.mode);

    result.append(node);
  }

  return result;
}

/**
 * A JSON response contains a percent of discrepancy two images in the data section:
 * [{discrepancy_percent}]
 */
Json::Value MessageFactory::Server::CreateGetImagesDiscrepancyResponse(int percent_discrepancy) {
  Json::Value result;

  result[kDiscrepancyPercent] = percent_discrepancy;

  return result;
}

/**
 * A JSON response contains a percent of discrepancy two images in the data section:
 * [{filename: [...]}]
 */
Json::Value MessageFactory::Server::CreateCaptureFramesResponse(const std::vector<std::string>& frame_list) {
  Json::Value result;
  for (const auto& frame : frame_list) {
    Json::Value node(frame);
    result[kFileName].append(node);
  }
  return result;
}

std::string MessageFactory::DBusConnection::CreateDisplayTypeChangedRequest(int x, int y, uint64_t id) {
  Json::Value params;
  Json::FastWriter writer;

  params[kAbscissa] = x;
  params[kOrdinate] = y;

  Json::Value message;
  CreatePackageStructure(message, kDisplayTypeChanged, id);
  message[kParams] = params;

  return writer.write(message);
}  // namespace jmsg

Json::Value MessageFactory::DBusConnection::CreateDisplayTypeChangedResponse() { return Json::Value{true}; }

std::string MessageFactory::SignalConnection::CreateIconReloadRequest(uint64_t id) {
  Json::FastWriter writer;

  Json::Value message;
  CreatePackageStructure(message, kReloadIconStorage, id);

  return writer.write(message);
}

Json::Value MessageFactory::SignalConnection::CreateIconReloadResponse() { return Json::Value{true}; }

}  // namespace jmsg
}  // namespace common
