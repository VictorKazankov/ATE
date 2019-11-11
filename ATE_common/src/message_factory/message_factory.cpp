#include "message_factory/message_factory.h"

#include <utility>

#include "json/writer.h"

#include "message_factory/json_defines.h"
#include "message_factory/json_messages.h"

namespace common {
namespace jmsg {

namespace {

void CreatePackageStructure(Json::Value& structure, const std::string& method, int id) {
  structure[kJsonRpc] = kRpcVersion;
  structure[kMethod] = method;
  structure[kId] = id;
}

}  // namespace

std::string MessageFactory::Client::CreateAttachToApplicationRequest(uint16_t timeout_msec, int id) {
  Json::Value message;
  Json::Value params;
  Json::FastWriter writer;

  params[kTimeoutMsec] = timeout_msec;

  CreatePackageStructure(message, kAttachToApplication, id);
  message[kParams] = params;

  return writer.write(message);
}

std::string MessageFactory::Client::CreateWaitForObjectRequest(const std::string& object_name, uint32_t timeout_msec,
                                                               int id) {
  Json::Value params;
  Json::FastWriter writer;

  params[kObjectName] = object_name;
  params[kTimeoutMsec] = timeout_msec;

  Json::Value message;

  CreatePackageStructure(message, kWaitForObject, id);
  message[kParams] = params;

  return writer.write(message);
}

std::string MessageFactory::Client::CreateTapObjectRequest(uint16_t x, uint16_t y, squish::ModifierState modifier_state,
                                                           squish::MouseButton mouse_button, int id) {
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

std::string MessageFactory::Client::CreateTouchAndDragRequest(const std::string& object_or_name, uint16_t x, uint16_t y,
                                                              int16_t dx, int16_t dy,
                                                              squish::ModifierState modifier_state, int id) {
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

std::string MessageFactory::Client::CreateChangeSyncIconDBRequest(const std::string& sync_version,
                                                                  const std::string& sync_build_version, int id) {
  Json::Value params;
  Json::FastWriter writer;

  params[kSyncVersion] = sync_version;
  params[kSyncBuildVersion] = sync_build_version;

  Json::Value message;
  CreatePackageStructure(message, kChangeSyncIconDB, id);
  message[kParams] = params;

  return writer.write(message);
}

std::string MessageFactory::Client::CreateChangeSyncModeRequest(const std::string& collection_mode, int id) {
  Json::Value params;
  Json::FastWriter writer;

  params[kSyncCollectionMode] = collection_mode;

  Json::Value message;

  CreatePackageStructure(message, kChangeSyncMode, id);
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

std::string MessageFactory::DBusConnection::CreateDisplayTypeChangedRequest(uint16_t x, uint16_t y, int id) {
  Json::Value params;
  Json::FastWriter writer;

  params[kAbscissa] = x;
  params[kOrdinate] = y;

  Json::Value message;
  CreatePackageStructure(message, kDisplayTypeChanged, id);
  message[kParams] = params;

  return writer.write(message);
}

Json::Value MessageFactory::DBusConnection::CreateDisplayTypeChangedResponse() { return Json::Value{true}; }

std::string MessageFactory::SignalConnection::CreateIconReloadRequest(int id) {
  Json::FastWriter writer;

  Json::Value message;
  CreatePackageStructure(message, kReloadIconStorage, id);

  return writer.write(message);
}

Json::Value MessageFactory::SignalConnection::CreateIconReloadResponse() { return Json::Value{true}; }

}  // namespace jmsg
}  // namespace common
