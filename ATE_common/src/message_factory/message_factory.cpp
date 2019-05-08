#include "message_factory/message_factory.h"

#include <jsoncpp/json/value.h>

#include "message_factory/json_defines.h"
#include "message_factory/json_messages.h"

namespace vhat_common {
namespace jmsg {

namespace {

void CreatePackageStructure(Json::Value& structure, const std::string& method, int id) {
  structure[kJsonRpc] = kRpcVersion;
  structure[kMethod] = method;
  structure[kId] = id;
}

}  // namespace

std::string MessageFactory::CreateAttachToApplicationRequest(uint16_t timeout, int id) {
  Json::Value message;
  Json::Value params;
  Json::FastWriter writer;

  params[kTimeout] = timeout;
  CreatePackageStructure(message, kAttachToApplication, id);
  message[kParams] = params;

  return writer.write(message);
}

std::string MessageFactory::CreateWaitForObjectRequest(const std::string& object_name, uint32_t timeout, int id) {
  Json::Value message;
  Json::Value params;
  Json::FastWriter writer;

  params[kObjectName] = object_name;
  params[kTimeout] = timeout;
  CreatePackageStructure(message, kWaitForObject, id);
  message[kParams] = params;

  return writer.write(message);
}

// TODO : (using of icon name and modifier) think about it!
std::string MessageFactory::CreateTapObjectRequest(uint16_t x, uint16_t y, int id) {
  Json::Value message;
  Json::Value params;
  Json::FastWriter writer;

  params[kAbscissa] = x;
  params[kOrdinate] = y;
  CreatePackageStructure(message, kTapObject, id);
  message[kParams] = params;

  return writer.write(message);
}

std::string MessageFactory::CreateTapObjectRequest(uint16_t x, uint16_t y, uint16_t width, uint16_t hight, int id) {
  Json::Value message;
  Json::Value params;
  Json::FastWriter writer;

  params[kAbscissa] = x;
  params[kOrdinate] = y;
  params[kWidth] = width;
  params[kHight] = hight;
  CreatePackageStructure(message, kTapObject, id);
  message[kParams] = params;

  return writer.write(message);
}

std::string MessageFactory::CreateAttachToApplicationResponse(const std::string& host, uint16_t port, bool is_running,
                                                              int id) {
  Json::Value message;
  Json::Value result;
  Json::FastWriter writer;

  result[kVdpHost] = host;
  result[kVdpPort] = port;
  result[kIsRunning] = is_running;
  CreatePackageStructure(message, kAttachToApplication, id);
  message[kResult] = result;

  return writer.write(message);
}

std::string MessageFactory::CreateWaitForObjectResponse(uint16_t x, uint16_t y, uint16_t width, uint16_t hight,
                                                        int id) {
  Json::Value message;
  Json::Value result;
  Json::FastWriter writer;

  result[kAbscissa] = x;
  result[kOrdinate] = y;
  result[kWidth] = width;
  result[kHight] = hight;
  CreatePackageStructure(message, kWaitForObject, id);
  message[kResult] = result;

  return writer.write(message);
}

}  // namespace jmsg
}  // namespace vhat_common
