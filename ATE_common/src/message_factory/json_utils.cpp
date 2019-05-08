#include "message_factory/json_utils.h"

#include <jsoncpp/json/reader.h>

#include "logger/logger.h"
#include "message_factory/json_defines.h"

namespace common {
namespace jmsg {

namespace {

const uint kPoint = 2;
const uint kRect = 4;

}  // namespace

bool CheckPoint(const Json::Value& params) {
  if (!(params.isMember(kAbscissa) && params.isMember(kOrdinate))) {
    logger::error("[json msg parser][check point] Argument error: wrong arguments for calling 'TapObject'");
    return false;
  }

  auto& abscissa = params[kAbscissa];
  auto& ordinate = params[kOrdinate];

  if (!(abscissa.isUInt() && ordinate.isUInt())) {
    logger::error("[json msg parser][check point] Argument error: wrong type of params");
    return false;
  }
  return true;
}

bool CheckRect(const Json::Value& params) {
  if (!(params.isMember(kAbscissa) && params.isMember(kOrdinate) && params.isMember(kWidth) &&
        params.isMember(kHight))) {
    logger::error("[json msg parser][check rect] Argument error: wrong arguments for calling 'TapObject'");
    return false;
  }

  auto& abscissa = params[kAbscissa];
  auto& ordinate = params[kOrdinate];
  auto& width = params[kWidth];
  auto& hight = params[kHight];

  if (!(abscissa.isUInt() && ordinate.isUInt() && width.isUInt() && hight.isUInt())) {
    logger::error("[json msg parser][check rect] Argument error: wrong type of params");
    return false;
  }
  return true;
}

bool ParseJson(const std::string& json_message, Json::Value& value) {
  Json::Reader reader;
  bool res = reader.parse(json_message, value);
  if (!res) {
    logger::error("[json msg parser][parse json] Parse error: {}", reader.getFormattedErrorMessages());
  }
  return res;
}

bool CheckHeader(const Json::Value& value) {
  bool res = value.isMember(kMethod) && value.isMember(kId) && value.isMember(kJsonRpc) && value.isMember(kParams);
  if (!res) {
    logger::error("[json msg parser][check header] Value error: wrong header");
  }
  return res;
}

bool CheckHeaderType(const Json::Value& value) {
  auto& message = value[kMethod];
  auto& id = value[kId];
  auto& rpc = value[kJsonRpc];

  bool res = message.isString() && id.isInt() && rpc.isString();
  if (!res) {
    logger::error("[json msg parser][check header type] Type error: wrong header");
    return res;
  }

  res = rpc.asString() == kRpcVersion;
  if (!res) {
    logger::error("[json msg parser][check header type] Incorrect version of jsonrpc");
  }
  return res;
}

bool CheckAttachToApplicationRequest(const Json::Value& schema) {
  auto& params = schema[kParams];
  bool res = params.isMember(kTimeoutMsec);

  if (!res) {
    logger::error(
        "[json msg parser][check app launch] Argument error: wrong arguments for calling "
        "'WaitForappLaunch");
    return res;
  }

  auto& timeout = params[kTimeoutMsec];

  res = timeout.isUInt();
  if (!res) {
    logger::error("[json msg parser][check app launch] Argument error: wrong type of params");
  }
  return res;
}

bool CheckWaitForObjectRequest(const Json::Value& schema) {
  auto& params = schema[kParams];
  bool res = params.isMember(kObjectName) && params.isMember(kTimeoutMsec);

  if (!res) {
    logger::error(
        "[json msg parser][check wait for object] Argument error: wrong arguments for calling 'WaitForObject");
    return res;
  }

  auto& object_name = params[kObjectName];
  auto& timeout = params[kTimeoutMsec];

  res = object_name.isString() && timeout.isInt();
  if (!res) {
    logger::error("[json msg parser][check wait for object] Argument error: wrong type of params str: {} int:{}",
                  object_name.isString(), timeout.isInt());
  }
  return res;
}

bool CheckTapObjectRequest(const Json::Value& schema) {
  auto& params = schema[kParams];

  switch (params.size()) {
    case kRect:
      return CheckRect(params);
    case kPoint:
      return CheckPoint(params);
    default:
      logger::error("[json msg parser][check tap object] Argument error: wrong count of arguments");
      return false;
  }
}

bool CheckAttachToApplicationResponse(const Json::Value& schema) {
  auto& result = schema[kResult];
  bool res = result.isMember(kVdpHost) && result.isMember(kVdpPort) && result.isMember(kIsRunning);

  if (!res) {
    logger::error(
        "[json msg parser][check app launch] Argument error: wrong type of response "
        "'WaitForApplicationLaunch'");
    return res;
  }

  auto& host = result[kVdpHost];
  auto& port = result[kVdpPort];
  auto& is_running = result[kIsRunning];

  res = host.isString() && port.isUInt() && is_running.isBool();
  if (!res) {
    logger::error(
        "[json msg parser][check app launch] Argument error: wrong type of params 'WaitForApplicationLaunch' "
        "response");
  }
  return res;
}

bool CheckWaitForObjectResponse(const Json::Value& schema) {
  auto& result = schema[kResult];
  bool res =
      result.isMember(kAbscissa) && result.isMember(kOrdinate) && result.isMember(kWidth) && result.isMember(kHight);

  if (!res) {
    logger::error("[json msg parser][check wait for object] Argument error: wrong type of response 'WaitForObject'");
    return res;
  }

  auto& abscissa = result[kAbscissa];
  auto& ordinate = result[kOrdinate];
  auto& width = result[kWidth];
  auto& hight = result[kHight];

  res = abscissa.isUInt() && ordinate.isUInt() && width.isUInt() && hight.isUInt();
  if (!res) {
    logger::error("[json msg parser][check rect] Argument error: wrong type of params of 'WaitForObject' response");
  }
  return res;
}

}  // namespace jmsg
}  // namespace common
