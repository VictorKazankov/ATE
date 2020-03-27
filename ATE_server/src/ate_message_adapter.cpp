#include "ate_message_adapter.h"

#include <cassert>
#include <utility>
#include <experimental/filesystem>

#include "json/value.h"

#include "ate_error.h"
#include "logger/logger.h"
#include "message_factory/json_defines.h"
#include "message_factory/json_messages.h"
#include "message_factory/json_utils.h"
#include "message_factory/message_factory.h"
#include "rpc_error.h"
#include "utils/squish_types.h"

namespace fs = std::experimental::filesystem;

AteMessageAdapter::AteMessageAdapter(ATE& ate)
    : ate_(ate),
      handler_map_{{common::jmsg::kWaitForObject, &AteMessageAdapter::HandleWaitForObject},
                   {common::jmsg::kTapObject, &AteMessageAdapter::HandleTapObject},
                   {common::jmsg::kTouchAndDrag, &AteMessageAdapter::HandleTouchAndDrag},
                   {common::jmsg::kPressAndHold, &AteMessageAdapter::HandlePressAndHold},
                   {common::jmsg::kPressRelease, &AteMessageAdapter::HandlePressRelease},
                   {common::jmsg::kDisplayTypeChanged, &AteMessageAdapter::HandleDisplayTypeChanged},
                   {common::jmsg::kChangeSyncIconDB, &AteMessageAdapter::HandleChangeSyncIconDB},
                   {common::jmsg::kChangeSyncMode, &AteMessageAdapter::HandleChangeSyncMode},
                   {common::jmsg::kReloadIconStorage, &AteMessageAdapter::HandleReloadIconStorage},
                   {common::jmsg::kLongPress, &AteMessageAdapter::HandleLongPress},
                   {common::jmsg::kGetScreenshot, &AteMessageAdapter::HandleGetScreenshot},
                   {common::jmsg::kGetText, &AteMessageAdapter::HandleGetText},
                   {common::jmsg::kGetObjectsDataByPattern, &AteMessageAdapter::HandleGetObjectsDataByPattern},
                   {common::jmsg::kGetImagesDiscrepancy, &AteMessageAdapter::GetImagesDiscrepancy}} {}

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
  common::ObjectDataIdentity object_data_identity;
  std::chrono::milliseconds timeout;
  Json::Value error;

  const bool is_wildcard = params.isMember(common::jmsg::kName) && params.isMember(common::jmsg::kSyncVersion) &&
                           params.isMember(common::jmsg::kSyncBuildVersion) &&
                           params.isMember(common::jmsg::kParentScreen) &&
                           params.isMember(common::jmsg::kSyncCollectionMode);

  common::jmsg::ExtractWaitForObjectRequestParams(params, object_data_identity, timeout, error);

  if (!error.empty()) {
    // Extract error occurs
    return std::make_pair(std::move(error), false);
  }

  cv::Rect position;
  std::error_code error_code;
  std::tie(position, error_code) = is_wildcard ? ate_.WaitForObject(object_data_identity, timeout)
                                               : ate_.WaitForObject(object_data_identity.name, timeout);

  // TODO: replace by switch! Hint: return from waitForObject enum
  if (error_code) {
    if (error_code == common::AteError::kVideoTemporarilyUnavailable) {
      error = common::jmsg::CreateErrorObject(rpc::Error::kVideoStreamNotFound, error_code.message().c_str());
    } else if (error_code == common::AteError::kPatternInvalid || error_code == common::AteError::kPatternNotFound) {
      error = common::jmsg::CreateErrorObject(rpc::Error::kObjectNotFound, error_code.message().c_str());
    } else {
      logger::warn("[ate message adapter] unhandled error: {}, treated as object not found error");
      error = common::jmsg::CreateErrorObject(rpc::Error::kInternalError, error_code.message().c_str());
    }
    logger::info("[ate message adapter] object_or_name: {}, timeout: {}ms error: {}", object_data_identity.name,
                 timeout.count(), error.toStyledString());
    return std::make_pair(std::move(error), false);
  }

  return std::make_pair(common::jmsg::MessageFactory::Server::CreateWaitForObjectResultObject(
                            position.x, position.y, position.width, position.height),
                        true);
}

std::pair<Json::Value, bool> AteMessageAdapter::HandleTapObject(const Json::Value& params) {
  cv::Point point;
  common::squish::ModifierState modifier_state = common::squish::ModifierState::kNone;
  common::squish::MouseButton mouse_button = common::squish::MouseButton::kNone;
  Json::Value extract_error;

  common::jmsg::ExtractTapObjectRequestParams(params, point.x, point.y, modifier_state, mouse_button, extract_error);

  if (!extract_error.empty()) {
    return std::make_pair(std::move(extract_error), false);
  }

  // TODO: we do not provide modifier state and mouse button in TapObject API
  ate_.TapObject(point);

  return std::make_pair(common::jmsg::MessageFactory::Server::CreateTapObjectResultObject(), true);
}

std::pair<Json::Value, bool> AteMessageAdapter::HandleLongPress(const Json::Value& params) {
  uint16_t x;
  uint16_t y;
  std::chrono::milliseconds timeout;
  Json::Value error;

  common::jmsg::ExtractLongPressRequestParams(params, x, y, timeout, error);

  if (!error.empty()) {
    return std::make_pair(std::move(error), false);
  }

  std::error_code error_code;
  error_code = ate_.LongPress(x, y, timeout);

  if (error_code == common::AteError::kInvalidDurationLongPress) {
    error = common::jmsg::CreateErrorObject(rpc::Error::kInvalidDurationLongPress, error_code.message().c_str());
  }

  if (!error.empty()) {
    logger::info("[ate message adapter] Press is too long {}ms error: {}", timeout.count(), error.toStyledString());
    return std::make_pair(std::move(error), false);
  }

  return std::make_pair(common::jmsg::MessageFactory::Server::CreateLongPressResultObject(), true);
}

std::pair<Json::Value, bool> AteMessageAdapter::HandleTouchAndDrag(const Json::Value& params) {
  std::string object_or_name;
  cv::Point start_point;
  cv::Point delta_point;
  common::squish::ModifierState modifier_state = common::squish::ModifierState::kNone;
  Json::Value extract_error;

  common::jmsg::ExtractTouchAndDragRequestParams(params, object_or_name, start_point.x, start_point.y, delta_point.x,
                                                 delta_point.y, modifier_state, extract_error);

  if (!extract_error.empty()) {
    return std::make_pair(std::move(extract_error), false);
  }

  ate_.TouchAndDrag(object_or_name, start_point, delta_point);

  return std::make_pair(common::jmsg::MessageFactory::Server::CreateTouchAndDragResultObject(), true);
}

std::pair<Json::Value, bool> AteMessageAdapter::HandlePressAndHold(const Json::Value& params) {
  cv::Point point;
  Json::Value extract_error;

  common::jmsg::ExtractPressAndHoldRequestParams(params, point.x, point.y, extract_error);

  if (!extract_error.empty()) {
    return std::make_pair(std::move(extract_error), false);
  }

  ate_.PressAndHold(point);

  return std::make_pair(common::jmsg::MessageFactory::Server::CreatePressAndHoldResultObject(), true);
}

std::pair<Json::Value, bool> AteMessageAdapter::HandlePressRelease(const Json::Value& params) {
  cv::Point point;
  Json::Value extract_error;

  common::jmsg::ExtractPressReleaseRequestParams(params, point.x, point.y, extract_error);

  if (!extract_error.empty()) {
    return std::make_pair(std::move(extract_error), false);
  }

  ate_.PressRelease(point);

  return std::make_pair(common::jmsg::MessageFactory::Server::CreatePressReleaseResultObject(), true);
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

std::pair<Json::Value, bool> AteMessageAdapter::HandleGetScreenshot(const Json::Value& params) {
  Json::Value result_error;

  std::string filename;
  std::string location;

  common::jmsg::ExtractGetScreenshotParams(params, filename, location, result_error);

  if (!result_error.empty()) {
    return std::make_pair(std::move(result_error), false);
  }

  std::error_code error_code = ate_.GetScreenshot(location, filename);

  if (error_code) {
    if (error_code == common::AteError::kVideoTemporarilyUnavailable) {
      result_error = common::jmsg::CreateErrorObject(rpc::Error::kVideoStreamNotFound, error_code.message().c_str());
    } else if (error_code == common::AteError::kEmptyFileName) {
      result_error = common::jmsg::CreateErrorObject(rpc::Error::kEmptyFileName, error_code.message().c_str());
    } else if (error_code == common::AteError::kWrongExtension) {
      result_error = common::jmsg::CreateErrorObject(rpc::Error::kWrongExtension, error_code.message().c_str());
    } else if (error_code == common::AteError::kPermissionDenied) {
      result_error = common::jmsg::CreateErrorObject(rpc::Error::kPermissionDenied, error_code.message().c_str());
    } else if (error_code == common::AteError::kImageAssemblingFailed) {
      result_error = common::jmsg::CreateErrorObject(rpc::Error::kImageAssemblingFailed, error_code.message().c_str());
    } else {
      logger::warn("[ate message adapter] unhandled error {}", error_code.message());
      result_error = common::jmsg::CreateErrorObject(rpc::Error::kInternalError, error_code.message().c_str());
    }
    return std::make_pair(std::move(result_error), false);
  }

  return std::make_pair(common::jmsg::MessageFactory::Server::CreateGetScreenshotObject, true);
}

std::pair<Json::Value, bool> AteMessageAdapter::HandleGetText(const Json::Value& params) {
  Json::Value result_error;

  common::Point top_left{}, bottom_right{};

  common::jmsg::ExtractGetTextRequestParams(params, top_left, bottom_right, result_error);
  if (!result_error.empty()) {
    return std::make_pair(std::move(result_error), false);
  }

  if (top_left.x == bottom_right.x || top_left.y == bottom_right.y) {
    result_error = common::jmsg::CreateErrorObject(rpc::Error::kInvalidRectangleCoordinates,
                                                   "Invalid GetText coordinates: zero height or width");
    return std::make_pair(std::move(result_error), false);
  }

  if (top_left.x > bottom_right.x || top_left.y > bottom_right.y) {
    result_error =
        common::jmsg::CreateErrorObject(rpc::Error::kInvalidRectangleCoordinates,
                                        "Invalid GetText coordinates: mixed up topleft and bottom-right points");

    return std::make_pair(std::move(result_error), false);
  }

  auto res = ate_.GetText(top_left, bottom_right);

  if (res.second) {
    if (common::AteError::kVideoTemporarilyUnavailable == res.second) {
      result_error = common::jmsg::CreateErrorObject(rpc::Error::kVideoStreamNotFound, res.second.message().data());
      return std::make_pair(std::move(result_error), false);
    } else if (common::AteError::kOutOfBoundaries == res.second) {
      result_error = common::jmsg::CreateErrorObject(rpc::Error::kInvalidRectangleCoordinates,
                                                     "Specified area is out of screen boundaries");
      return std::make_pair(std::move(result_error), false);
    }
  }

  return std::make_pair(common::jmsg::MessageFactory::Server::CreateGetTextResultObject(res.first), true);
}

std::pair<Json::Value, bool> AteMessageAdapter::HandleGetObjectsDataByPattern(const Json::Value& params) {
  Json::Value error;
  common::ObjectDataIdentity object_data_identity;

  common::jmsg::ExtractGetObjectsDataByPatternParams(params, object_data_identity, error);

  if (!error.empty()) {
    // Extract error occurs
    return std::make_pair(std::move(error), false);
  }

  auto res = ate_.GetObjectsDataByPattern(object_data_identity);

  return std::make_pair(common::jmsg::MessageFactory::Server::CreateGetObjectsDataByPatternResponse(res), true);
}

std::pair<Json::Value, bool> AteMessageAdapter::GetImagesDiscrepancy(const Json::Value& params) {
  Json::Value error;
  std::string p2;
  std::string p1;
  common::Point top_left_coordinate;
  common::Point bottom_right_coordinate;

  common::jmsg::ExtractGetImagesDiscrepancyParams(params, p2, p1, top_left_coordinate, bottom_right_coordinate, error);

  // Creating the correct path to the images
  fs::path icon_path_second{ATE_WRITABLE_DATA_PREFIX};
  icon_path_second /= p2;

  fs::path icon_path_first{ATE_WRITABLE_DATA_PREFIX};
  icon_path_first /= p1;

  if (!error.empty()) {
    // Extract error occurs
    return std::make_pair(std::move(error), false);
  }

  // Validation input parameters
  std::error_code error_code;

  if (p2.empty() || p1.empty()) {
    error = common::jmsg::CreateErrorObject(rpc::Error::kEmptyFileName, "The screenshot's filename is empty");
    return std::make_pair(std::move(error), false);
  }

  if (!fs::is_regular_file(icon_path_second, error_code) || !fs::exists(icon_path_second, error_code) ||
      !fs::is_regular_file(icon_path_first, error_code) || !fs::exists(icon_path_first, error_code)) {
    error = common::jmsg::CreateErrorObject(rpc::Error::kComparingImageNotExist, "Comparing image doesn't exist");
    return std::make_pair(std::move(error), false);
  }

  // Check supported resolution
  std::string icon_first_ext = fs::path(icon_path_first).extension();
  std::transform(icon_first_ext.begin(), icon_first_ext.end(), icon_first_ext.begin(),
                 [](unsigned char c) -> unsigned char { return std::toupper(c); });
  std::string icon_second_ext = fs::path(icon_path_second).extension();
  std::transform(icon_second_ext.begin(), icon_second_ext.end(), icon_second_ext.begin(),
                 [](unsigned char c) -> unsigned char { return std::toupper(c); });
  std::vector<std::string> supported_resolution{".PNG", ".JPG", ".JPEG"};
  if (std::find(supported_resolution.begin(), supported_resolution.end(), icon_second_ext) ==
          supported_resolution.end() ||
      std::find(supported_resolution.begin(), supported_resolution.end(), icon_first_ext) ==
          supported_resolution.end()) {
    error = common::jmsg::CreateErrorObject(rpc::Error::kUnsupportedFileType,
                                            "Compared images have unsupported file types");
    return std::make_pair(std::move(error), false);
  }

  auto first_image_size = fs::file_size(icon_path_first, error_code);
  auto second_image_size = fs::file_size(icon_path_second, error_code);
  if (first_image_size == 0 || first_image_size == std::numeric_limits<uintmax_t>::max() || second_image_size == 0 ||
      second_image_size == std::numeric_limits<uintmax_t>::max()) {
    error =
        common::jmsg::CreateErrorObject(rpc::Error::kComparingImageIncorrectSize, "Comparing image is incorrect size");
    return std::make_pair(std::move(error), false);
  }

  if (top_left_coordinate.x > bottom_right_coordinate.x || top_left_coordinate.y > bottom_right_coordinate.y) {
    error =
        common::jmsg::CreateErrorObject(rpc::Error::kInvalidRectangleCoordinates,
                                        "Invalid Matching area coordinate: mixed up top-left and bottom-right points");
    return std::make_pair(std::move(error), false);
  }

  if ((top_left_coordinate.x == bottom_right_coordinate.x && top_left_coordinate.x > 0) ||
      (top_left_coordinate.y == bottom_right_coordinate.y && top_left_coordinate.y > 0)) {
    error =
        common::jmsg::CreateErrorObject(rpc::Error::kInvalidRectangleCoordinates,
                                        "Invalid Matching area coordinate: comparing rectangle has zero height/width");
    return std::make_pair(std::move(error), false);
  }

  auto first_image_permissions = fs::status(icon_path_first).permissions();
  auto second_image_permissions = fs::status(icon_path_second).permissions();
  if ((first_image_permissions & fs::perms::owner_read) == fs::perms::none ||
      (first_image_permissions & fs::perms::group_read) == fs::perms::none ||
      (first_image_permissions & fs::perms::others_read) == fs::perms::none ||
      (second_image_permissions & fs::perms::owner_read) == fs::perms::none ||
      (second_image_permissions & fs::perms::group_read) == fs::perms::none ||
      (second_image_permissions & fs::perms::others_read) == fs::perms::none) {
    error = common::jmsg::CreateErrorObject(rpc::Error::kPermissionDenied, "Invalid read permission");
    return std::make_pair(std::move(error), false);
  }

  // Get image discrepancy
  auto result =
      ate_.GetImagesDiscrepancy(icon_path_second, icon_path_first, top_left_coordinate, bottom_right_coordinate);

  if (result.second) {
    if (common::AteError::kOutOfBoundaries == result.second) {
      error = common::jmsg::CreateErrorObject(rpc::Error::kInvalidRectangleCoordinates,
                                              "Specified area is out of screen boundaries");
      return std::make_pair(std::move(error), false);
    }
    if (common::AteError::kSystemError == result.second) {
      error =
          common::jmsg::CreateErrorObject(rpc::Error::kInternalError, "Internal error during comparison of two images");
      return std::make_pair(std::move(error), false);
    }
    if (common::AteError::kWrongImageResolution == result.second) {
      error = common::jmsg::CreateErrorObject(rpc::Error::kComparingImageIncorrectSize,
                                              "The compared images have different resolutions");
      return std::make_pair(std::move(error), false);
    }
    error = common::jmsg::CreateErrorObject(rpc::Error::kInternalError, "Unexpected error.");
    return std::make_pair(std::move(error), false);
  }

  return std::make_pair(common::jmsg::MessageFactory::Server::CreateGetImagesDiscrepancyResponse(result.first), true);
}

std::pair<Json::Value, bool> AteMessageAdapter::HandleUnknownMethod(const Json::Value& params) {
  Json::Value error = common::jmsg::CreateErrorObject(rpc::Error::kMethodNotFound, "Method not found");
  logger::error("[ate message adapter] {}\nparams: {}", error.toStyledString(), params.toStyledString());
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
    case adapter::DBManagerError::kLogicError: {
      return common::jmsg::CreateErrorObject(rpc::Error::kInternalError, "Failed to check Icon storage configuration");
    }
    case adapter::DBManagerError::kSuccess: {
      break;
    }
  }

  return {};
}
