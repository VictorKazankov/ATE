#include "message_factory/json_utils.h"

#include <cassert>
#include <cstring>
#include <utility>

#include "json/reader.h"

#include "logger/logger.h"
#include "message_factory/json_defines.h"

namespace common {
namespace jmsg {

Json::Value CreateErrorObject(rpc::Error code, const char* message, Json::Value data) {
  Json::Value error_value{Json::objectValue};

  error_value[kErrorCode] = static_cast<int>(code);
  error_value[kErrorMessage] = message;

  if (!data.empty()) {
    std::swap(error_value[kErrorData], data);
  }

  return std::move(error_value);
}

bool ParseJson(const std::string& json_message, Json::Value& value) {
  Json::Reader reader;
  bool res = reader.parse(json_message, value);
  if (!res) {
    logger::error("[json msg parser][parse json] Parse error: {}", reader.getFormattedErrorMessages());
  }
  return res;
}

void ParseJsonRpcRequest(const std::string& json_message, std::uint64_t& id, std::string& method, Json::Value& params,
                         Json::Value& error) {
  id = 0;
  error = Json::Value{};

  Json::Reader reader;
  Json::Value message;

  if (!reader.parse(json_message, message, false)) {
    const auto error_message = reader.getFormattedErrorMessages();
    error = CreateErrorObject(rpc::Error::kParseError, error_message.c_str());
    logger::error("[json msg parser] Parse error: {}", error.toStyledString());
    return;
  }

  std::string id_error;
  if (!message[kId].isUInt64()) {
    id = 0;
    id_error.append("Wrong id: id must be unsigned interger");
  } else {
    id = message[kId].asUInt64();
  }

  if (!id) {
    error = CreateErrorObject(rpc::Error::kInvalidRequest, id_error.empty() ? "id must be non-zero" : id_error.c_str());
    logger::error("[json msg parser] {}id: {}", error.toStyledString(), message[kId].toStyledString());
    return;
  }

  try {
    if (std::strcmp(message[kJsonRpc].asCString(), kRpcVersion)) {
      error = CreateErrorObject(rpc::Error::kInvalidRequest, "Invalid jsonrpc version string");
      logger::error("[json msg parser] {}jsonrpc: {}", error.toStyledString(), message[kJsonRpc].toStyledString());
      return;
    }
    method = message[kMethod].asCString();
  } catch (const Json::LogicError& wrong_params) {
    error = CreateErrorObject(rpc::Error::kInvalidRequest, "Invalid request");
    logger::error("[json msg parser] {}message: {}({})", error.toStyledString(), message.toStyledString(),
                  wrong_params.what());
    return;
  }

  assert(error.empty());
  std::swap(params, message[kParams]);
}

bool CheckHeader(const Json::Value& value) {
  bool res = value.isMember(kId) && value.isMember(kJsonRpc) && (value.isMember(kResult) || value.isMember(kError));
  if (!res) {
    logger::error("[json msg parser][check header] Value error: wrong header");
  }
  return res;
}

bool CheckHeaderType(const Json::Value& value) {
  auto& id = value[kId];
  auto& rpc = value[kJsonRpc];

  bool res = id.isInt() && rpc.isString();
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

void ExtractWaitForObjectRequestParams(const Json::Value& params, std::string& object_or_name,
                                       std::chrono::milliseconds& timeout, Json::Value& error) {
  error = Json::Value{};

  try {
    object_or_name = params[kObjectName].asCString();
    timeout = std::chrono::milliseconds{params[kTimeoutMsec].asUInt()};
  } catch (const Json::LogicError& wrong_params) {
    error = CreateErrorObject(rpc::Error::kInvalidParams, "Invalid WaitForObject params");
    logger::error("[json msg parser] {}params: {}({})", error.toStyledString(), params.toStyledString(),
                  wrong_params.what());
  }
}

void ExtractTapObjectRequestParams(const Json::Value& params, int& x, int& y, squish::ModifierState& modifier_state,
                                   squish::MouseButton& mouse_button, Json::Value& error) {
  error = Json::Value{};

  try {
    x = params[kAbscissa].asInt();
    y = params[kOrdinate].asInt();
    modifier_state = static_cast<squish::ModifierState>(params[kModifierState].asInt());
    mouse_button = static_cast<squish::MouseButton>(params[kMouseButton].asInt());
  } catch (const Json::LogicError& wrong_params) {
    error = CreateErrorObject(rpc::Error::kInvalidParams, "Invalid TapObject params");
    logger::error("[json msg parser] {}params: {}({})", error.toStyledString(), params.toStyledString(),
                  wrong_params.what());
  }
}

void ExtractLongPressRequestParams(const Json::Value& params, uint16_t& x, uint16_t& y,
                                   std::chrono::milliseconds& timeout, Json::Value& error) {
  error = Json::Value{};

  try {
    x = params[kAbscissa].asUInt();
    y = params[kOrdinate].asUInt();
    timeout = std::chrono::milliseconds{params[kTimeoutMsec].asUInt()};
  } catch (const Json::LogicError& wrong_params) {
    error = CreateErrorObject(rpc::Error::kInvalidParams, "Invalid LongPress params");
    logger::error("[json msg parser] {}params: {}({})", error.toStyledString(), params.toStyledString(),
                  wrong_params.what());
  }
}

void ExtractTouchAndDragRequestParams(const Json::Value& params, std::string& object_or_name, int& x, int& y, int& dx,
                                      int& dy, squish::ModifierState& modifier_state, Json::Value& error) {
  error = Json::Value{};

  try {
    object_or_name = params[kObjectName].asCString();
    x = params[kAbscissa].asInt();
    y = params[kOrdinate].asInt();
    dx = params[kAbscissaDrag].asInt();
    dy = params[kOrdinateDrag].asInt();
    modifier_state = static_cast<squish::ModifierState>(params[kModifierState].asInt());
  } catch (const Json::LogicError& wrong_params) {
    error = CreateErrorObject(rpc::Error::kInvalidParams, "Invalid TouchAndDrag params");
    logger::error("[json msg parser] {}params: {}({})", error.toStyledString(), params.toStyledString(),
                  wrong_params.what());
  }
}

void ExtractPressAndHoldRequestParams(const Json::Value& params, int& x, int& y, Json::Value& error) {
  error = Json::Value{};

  try {
    x = params[kAbscissa].asInt();
    y = params[kOrdinate].asInt();
  } catch (const Json::LogicError& wrong_params) {
    error = CreateErrorObject(rpc::Error::kInvalidParams, "Invalid PressAndHold params");
    logger::error("[json msg parser] {} params: {}({})", error.toStyledString(), params.toStyledString(),
                  wrong_params.what());
  }
}

void ExtractPressReleaseRequestParams(const Json::Value& params, int& x, int& y, Json::Value& error) {
  error = Json::Value{};

  try {
    x = params[kAbscissa].asInt();
    y = params[kOrdinate].asInt();
  } catch (const Json::LogicError& wrong_params) {
    error = CreateErrorObject(rpc::Error::kInvalidParams, "Invalid PressRelease params");
    logger::error("[json msg parser] {} params: {}({})", error.toStyledString(), params.toStyledString(),
                  wrong_params.what());
  }
}

void ExtractGetScreenshotParams(const Json::Value& params, std::string& filename, std::string& location,
                                Json::Value& error) {
  error = Json::Value{};

  try {
    filename = params[kFileName].asString();
    location = params[kLocation].asString();
  } catch (const Json::LogicError& wrong_params) {
    error = CreateErrorObject(rpc::Error::kInvalidParams, "Invalid GetScreenshot params");
    logger::error("[json msg parser] {} params: {}({})", error.toStyledString(), params.toStyledString(),
                  wrong_params.what());
  }
}

void ExtractDisplayTypeChangedRequestParams(const Json::Value& params, int& x, int& y, Json::Value& error) {
  error = Json::Value{};
  try {
    x = params[kAbscissa].asInt();
    y = params[kOrdinate].asInt();
  } catch (const Json::LogicError& wrong_params) {
    error = CreateErrorObject(rpc::Error::kInvalidParams, "Invalid TouchAndDrag params");
    logger::error("[json msg parser] {}params: {}({})", error.toStyledString(), params.toStyledString(),
                  wrong_params.what());
  }
}

bool CheckWaitForObjectResponse(const Json::Value& schema) {
  auto& result = schema[kResult];
  bool res =
      result.isMember(kAbscissa) && result.isMember(kOrdinate) && result.isMember(kWidth) && result.isMember(kHeight);

  if (!res) {
    logger::error("[json msg parser][check wait for object] Argument error: wrong type of response 'WaitForObject'");
    return res;
  }

  auto& abscissa = result[kAbscissa];
  auto& ordinate = result[kOrdinate];
  auto& width = result[kWidth];
  auto& height = result[kHeight];

  res = abscissa.isUInt() && ordinate.isUInt() && width.isUInt() && height.isUInt();
  if (!res) {
    logger::error("[json msg parser][check rect] Argument error: wrong type of params of 'WaitForObject' response");
  }
  return res;
}

void ExtractChangeSyncIconDBRequestParams(const Json::Value& params, std::string& sync_version,
                                          std::string& sync_build_version, Json::Value& error) {
  error = Json::Value{};

  try {
    sync_version = params[kSyncVersion].asString();
    sync_build_version = params[kSyncBuildVersion].asString();
    if (sync_version.empty() || sync_build_version.empty()) {
      error = CreateErrorObject(rpc::Error::kInvalidParams, "Invalid ChangeSyncIconDB params");
      logger::error("[json msg parser] {}params: {}(empty sync_version or sync_build_version)", error.toStyledString(),
                    params.toStyledString());
    }
  } catch (const Json::LogicError& wrong_params) {
    error = CreateErrorObject(rpc::Error::kInvalidParams, "Invalid ChangeSyncIconDB params");
    logger::error("[json msg parser] {}params: {}({})", error.toStyledString(), params.toStyledString(),
                  wrong_params.what());
  }
}

void ExtractChangeSyncModeRequestParams(const Json::Value& params, std::string& collection_mode, Json::Value& error) {
  error = Json::Value{};

  try {
    collection_mode = params[kSyncCollectionMode].asString();
    if (collection_mode.empty()) {
      error = CreateErrorObject(rpc::Error::kInvalidParams, "Invalid SyncChangeMode params");
      logger::error("[json msg parser] {}params: {}(empty collection mode)", error.toStyledString(),
                    params.toStyledString());
    }
  } catch (const Json::LogicError& wrong_params) {
    error = CreateErrorObject(rpc::Error::kInvalidParams, "Invalid SyncChangeMode params");
    logger::error("[json msg parser] {}params: {}({})", error.toStyledString(), params.toStyledString(),
                  wrong_params.what());
  }
}

}  // namespace jmsg
}  // namespace common
