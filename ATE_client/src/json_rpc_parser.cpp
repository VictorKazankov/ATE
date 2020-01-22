#include "json_rpc_parser.h"

#include "logger/logger.h"
#include "message_factory/json_defines.h"
#include "message_factory/json_messages.h"
#include "message_factory/json_utils.h"

#include "error_defines.h"

using namespace interaction;

namespace {

/**
 * @brief The function verifies is message with JSON valid
 * @return true in case of valid JSON, otherwise false
 **/
bool IsJsonStructureValid(const std::string& message, Json::Value& value) {
  if (!common::jmsg::ParseJson(message, value)) {
    return false;
  }

  return common::jmsg::CheckHeader(value) && common::jmsg::CheckHeaderType(value);
}

/**
 * @brief The function handles errors related to JSON structure. In case of error, it throws the appropriate exception
 */
[[noreturn]] void ErrorHandler(const Json::Value& error) {
  switch (static_cast<rpc::Error>(error[common::jmsg::kErrorCode].asInt())) {
    case rpc::Error::kObjectNotFound:
      throw squish::LookupError{};

    case rpc::Error::kVideoStreamNotFound:
      throw VideoStreamingError{};

    case rpc::Error::kInternalError:
    case rpc::Error::kParseError:
    case rpc::Error::kInvalidRequest:
    case rpc::Error::kMethodNotFound:
      throw std::runtime_error(error[common::jmsg::kErrorMessage].asString());

    case rpc::Error::kInvalidParams:
      throw std::invalid_argument(error[common::jmsg::kErrorMessage].asString());

    case rpc::Error::kInvalidSyncVersion:
      throw squish::InvalidSyncVersion{};

    case rpc::Error::kInvalidSyncBuildVersion:
      throw squish::InvalidSyncBuildVersion{};

    case rpc::Error::kInvalidSyncCollectionMode:
      throw squish::InvalidSyncCollectionMode{};

    case rpc::Error::kInvalidDurationLongPress:
      throw squish::InvalidDurationLongPress{};

    case rpc::Error::kEmptyFileName:
      throw squish::EmptyScreenshotFileName{};

    case rpc::Error::kWrongExtension:
      throw squish::WrongScreenshotExtension{};

    case rpc::Error::kPermissionDenied:
      throw squish::PermissionDenied{};

    case rpc::Error::kImageAssemblingFailed:
      throw squish::ImageAssemblingFailed{};

    case rpc::Error::kInvalidRectangleCoordinates:
      throw squish::InvalidRectangleCoordinates{};

    default:
      throw std::runtime_error("Undefined error occurred");
  }
}
}  // namespace

Json::Value JsonRpcParser::RpcStringToJsonStruct(const std::string& rpc) {
  Json::Value schema;

  if (IsJsonStructureValid(rpc, schema)) {
    if (schema.isMember(common::jmsg::kError)) {
      ErrorHandler(schema[common::jmsg::kError]);
    }
  }

  return schema;
}

void JsonRpcParser::CheckAndRaiseExceptionInCaseErrors(const std::string& rpc) { RpcStringToJsonStruct(rpc); }

squish::Object JsonRpcParser::ParseWaitForObject(const std::string& rpc) {
  Json::Value schema = RpcStringToJsonStruct(rpc);

  squish::Object object;

  if (common::jmsg::CheckWaitForObjectResponse(schema)) {
    logger::debug("[parse message] waitForObject response");

    auto& result = schema[common::jmsg::kResult];

    object.x = result[common::jmsg::kAbscissa].asInt();
    object.y = result[common::jmsg::kOrdinate].asInt();
    object.width = result[common::jmsg::kWidth].asInt();
    object.height = result[common::jmsg::kHeight].asInt();
  }

  return object;
}

bool JsonRpcParser::ParseGetScreenshot(const std::string& rpc) {
  auto schema = RpcStringToJsonStruct(rpc);

  if (common::jmsg::CheckGetScreenshotResponse(schema)) {
    auto& result = schema[common::jmsg::kResult];
    return result.asBool();
  }
  return false;
}

std::string JsonRpcParser::ParseGetText(const std::string& rpc) {
  Json::Value schema;

  if (!IsJsonStructureValid(rpc, schema)) return {};

  // check response parameters
  if (schema.isMember(common::jmsg::kError)) {
    ErrorHandler(schema[common::jmsg::kError]);
  }
  const auto& result = schema[common::jmsg::kResult];
  if (!result.isMember(common::jmsg::kText)) {
    logger::error("[json msg parser] Argument error: wrong type of response 'GetText'");
    return {};
  }

  return result[common::jmsg::kText].asString();
}
