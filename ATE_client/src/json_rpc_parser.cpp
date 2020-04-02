#include "json_rpc_parser.h"

#include "utils/squish_types_converter.h"

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

    case rpc::Error::kInvalidDuration:
      throw squish::InvalidDuration{};

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

    case rpc::Error::kComparingImageIncorrectSize:
      throw squish::ComparingImageIncorrectSize{};

    case rpc::Error::kComparingImageNotExist:
      throw squish::ComparingImageNotExist{};

    case rpc::Error::kUnsupportedFileType:
      throw squish::UnsupportedFileType{};

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

  return result[common::jmsg::kText].asCString();
}

/**
 * A JSON response contains an array of next structures of objects in data section:
 * [{x, y, width, height, x_top_left, y_top_left, x_bottom_right, y_bottom_right, parent_width, parent_height, name,
 * parent_screen}]
 */
std::vector<squish::Object> JsonRpcParser::ParseGetObjectsDataByPattern(const std::string& rpc) {
  std::vector<squish::Object> objects_list;

  // If Error - thrown exception
  Json::Value schema = RpcStringToJsonStruct(rpc);

  // Parse
  auto& result = schema[common::jmsg::kResult];
  for (const auto& node : result) {
    squish::Object object;
    try {
      // Validating mandatory data fields
      if (!node.isMember(common::jmsg::kAbscissa) || !node.isMember(common::jmsg::kOrdinate) ||
          !node.isMember(common::jmsg::kWidth) || !node.isMember(common::jmsg::kHeight)) {
        logger::error(
            "[ParseGetObjectsDataByPattern] Invalid structure of object data in the node. Mandatory fields is "
            "incorrect.");
        continue;
      }

      // Extracting mandatory fields
      object.x = node[common::jmsg::kAbscissa].asInt();
      object.y = node[common::jmsg::kOrdinate].asInt();
      object.width = node[common::jmsg::kWidth].asInt();
      object.height = node[common::jmsg::kHeight].asInt();

      // Extracting optional fields
      if (node.isMember(common::jmsg::kXTopLeft) && node[common::jmsg::kXTopLeft].isInt()) {
        object.x_top_left = node[common::jmsg::kXTopLeft].asInt();
      }
      if (node.isMember(common::jmsg::kYTopLeft) && node[common::jmsg::kYTopLeft].isInt()) {
        object.y_top_left = node[common::jmsg::kYTopLeft].asInt();
      }
      if (node.isMember(common::jmsg::kXBottomRight) && node[common::jmsg::kXBottomRight].isInt()) {
        object.x_bottom_right = node[common::jmsg::kXBottomRight].asInt();
      }
      if (node.isMember(common::jmsg::kYBottomRight) && node[common::jmsg::kYBottomRight].isInt()) {
        object.y_bottom_right = node[common::jmsg::kYBottomRight].asInt();
      }
      if (node.isMember(common::jmsg::kParentWidth) && node[common::jmsg::kParentWidth].isInt()) {
        object.parent_width = node[common::jmsg::kParentWidth].asInt();
      }
      if (node.isMember(common::jmsg::kParentHeight) && node[common::jmsg::kParentHeight].isInt()) {
        object.parent_height = node[common::jmsg::kParentHeight].asInt();
      }
      if (node.isMember(common::jmsg::kName) && node[common::jmsg::kName].isString()) {
        object.name = node[common::jmsg::kName].asCString();
      }
      if (node.isMember(common::jmsg::kParentScreen) && node[common::jmsg::kParentScreen].isString()) {
        object.parent_screen = node[common::jmsg::kParentScreen].asCString();
      }
      if (node.isMember(common::jmsg::kSyncVersion) && node[common::jmsg::kSyncVersion].isString()) {
        object.sync_version = node[common::jmsg::kSyncVersion].asCString();
      }
      if (node.isMember(common::jmsg::kSyncBuildVersion) && node[common::jmsg::kSyncBuildVersion].isString()) {
        object.build_version = node[common::jmsg::kSyncBuildVersion].asCString();
      }
      if (node.isMember(common::jmsg::kSyncCollectionMode) && node[common::jmsg::kSyncCollectionMode].isString()) {
        object.mode = common::squish::StrToCollectionMode(node[common::jmsg::kSyncCollectionMode].asCString());
      }

      // Save object
      objects_list.push_back(std::move(object));
    } catch (const Json::LogicError& err) {
      logger::error("[ParseGetObjectsDataByPattern] Argument error: wrong type of response {}", err.what());
    }
  }

  return objects_list;
}

/**
 * A JSON response contains a percent of discrepancy two images in the data section:
 * [{discrepancy_percent}]
 */
int JsonRpcParser::ParseGetImagesDiscrepancy(const std::string& rpc) {
  Json::Value schema = RpcStringToJsonStruct(rpc);

  // Parse
  const auto& result = schema[common::jmsg::kResult];

  // Validating mandatory data fields
  if (!result.isMember(common::jmsg::kDiscrepancyPercent) || !result[common::jmsg::kDiscrepancyPercent].isInt() ||
      !result[common::jmsg::kDiscrepancyPercent].isUInt()) {
    logger::error(
        "[GetImagesDiscrepancy] Invalid structure of object data in the JSON response. Mandatory field is "
        "incorrect.");
    throw std::runtime_error("Missing mandatory field in the response from the server.");
  }

  // Extracting fields
  int discrepancy_percent = 100;
  try {
    discrepancy_percent = result[common::jmsg::kDiscrepancyPercent].asInt();
  } catch (const Json::LogicError& err) {
    logger::error("[GetImagesDiscrepancy] Argument error: wrong type of response {}", err.what());
    throw std::runtime_error("Error during parsing response. Wrong type of response");
  }

  return discrepancy_percent;
}

/*
 * A JSON response contains an array of next structures of objects in data section:
 * [{filename: [...]}]
 */
std::vector<std::string> JsonRpcParser::ParseCaptureFrames(const std::string& rpc) {
  // If Error - thrown exception
  Json::Value schema = RpcStringToJsonStruct(rpc);

  std::vector<std::string> file_list;

  // Parse
  auto& result = schema[common::jmsg::kResult];
  if (result.isMember(common::jmsg::kFileName)) {
    for (const auto& node : result[common::jmsg::kFileName]) {
      try {
        file_list.emplace_back(node.asCString());
      } catch (const Json::LogicError& err) {
        logger::error("[ParseCaptureFrames] Argument error: wrong type of response {}", err.what());
      }
    }
  }
  return file_list;
}
