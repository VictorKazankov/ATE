#include "adapters/db_manager_adapter.h"

#include <algorithm>
#include <cassert>
#include <cctype>
#include <stdexcept>

#include <db_manager/icon_data.h>
#include <opencv2/imgcodecs.hpp>

#include "logger/logger.h"
#include "utils/squish_types.h"

namespace adapter {

using namespace db_manager;

namespace converters {
template <typename ToType, typename FromType>
ToType ConvertMode(FromType mode) {
  switch (mode) {
    case FromType::kNone:
      return ToType::kNone;
    case FromType::kDay:
      return ToType::kDay;
    case FromType::kNight:
      return ToType::kNight;
    case FromType::kAny:
      return ToType::kAny;
  }

  logger::critical("[DBManagerAdapter] inexistent conversion between modes. Trying to convert from: {}",
                   static_cast<int>(mode));
  throw std::logic_error{"DBManagerAdapter: inexistent conversion between modes"};
}

constexpr auto SquishModeToDbMode = ConvertMode<HmiMode, common::squish::CollectionMode>;
constexpr auto DbModeToSquishMode = ConvertMode<common::squish::CollectionMode, HmiMode>;

HmiMode ConfigStringToDbMode(std::string mode) {
  std::transform(mode.begin(), mode.end(), mode.begin(), [](unsigned char c) { return std::toupper(c); });
  if (mode == "DAY_MODE" || mode == "DAY") {
    return HmiMode::kDay;
  } else if (mode == "NIGHT_MODE" || mode == "NIGHT") {
    return HmiMode::kNight;
  } else {
    logger::critical(
        "[DBManagerAdapter] Failed to convert configured mode {} to db_manager::HmiMode. Defaulting to HmiMode::kNone.",
        mode);
    return HmiMode::kNone;
  }
}

IconWildcard ObjectIdentityToDbWildcard(const common::ObjectDataIdentity& object_identity) {
  return IconWildcard{object_identity.sync_version, object_identity.build_version,
                      SquishModeToDbMode(object_identity.mode), object_identity.name, object_identity.parent_screen};
}

common::ObjectData DbIconDataToObjectData(const IconData& icon) {
  common::ObjectData result;

  result.name = icon.GetName();
  result.sync_version = icon.GetSyncVersion();
  result.build_version = icon.GetBuildVersion();
  result.mode = DbModeToSquishMode(icon.GetHmiMode());
  result.center = icon.GetCenter();

  if (icon.GetParentName()) result.parent_screen = icon.GetParentName().value();
  if (icon.GetTopLeft()) result.top_left = icon.GetTopLeft().value();
  if (icon.GetBottomRight()) result.bottom_right = icon.GetBottomRight().value();

  if (icon.GetTopLeft() && icon.GetBottomRight()) {
    result.width = icon.GetBottomRight().value().x - icon.GetTopLeft().value().x;
    result.height = icon.GetBottomRight().value().y - icon.GetTopLeft().value().y;
  }

  if (icon.GetParentResolution()) {
    result.parent_width = icon.GetParentResolution().value().width;
    result.parent_height = icon.GetParentResolution().value().height;
  }

  return result;
}
}  // namespace converters

DBManagerError DBManagerAdapter::ValidateConfiguration(const StorageConfig& config) const {
  if (config.sync_version.empty()) {
    logger::critical("[DBManagerAdapter] ValidateConfiguration(): invalid config.sync_version value: {}",
                     config.sync_version);
    return DBManagerError::kInvalidSyncVersion;
  }
  if (config.build_version.empty()) {
    logger::critical("[DBManagerAdapter] ValidateConfiguration(): invalid config.build_version value: {}",
                     config.build_version);
    return DBManagerError::kInvalidSyncBuildVersion;
  }
  if (config.mode != HmiMode::kDay && config.mode != HmiMode::kNight) {
    logger::critical("[DBManagerAdapter] ValidateConfiguration(): invalid config.mode value: {} (aka {})",
                     static_cast<int>(config.mode), HmiModeToString(config.mode));
    return DBManagerError::kInvalidCollectionMode;
  }
  return DBManagerError::kSuccess;
}

DBManagerError DBManagerAdapter::CheckConfiguration(const StorageConfig& config) const {
  assert(icon_data_mapper_);
  if (!icon_data_mapper_) {
    logger::critical("[DBManagerAdapter] CheckConfiguration(): icon_data_mapper_ was not created");
    return DBManagerError::kLogicError;
  }

  auto match_result = icon_data_mapper_->Match({config.sync_version, config.build_version, config.mode, "*", "*"});
  if (!match_result) {
    logger::critical("[DBManagerAdapter] CheckConfiguration(): icon_data_mapper_->Match() returned nullptr");
    return DBManagerError::kLogicError;
  }

  if (!match_result->HasNext()) {
    match_result = icon_data_mapper_->Match({config.sync_version, "*", HmiMode::kAny, "*", "*"});
    if (!match_result || !match_result->HasNext()) {
      logger::error("[DBManagerAdapter] Invalid SYNC product name: {}", config.sync_version);
      return DBManagerError::kInvalidSyncVersion;
    }

    match_result = icon_data_mapper_->Match({config.sync_version, config.build_version, HmiMode::kAny, "*", "*"});
    if (!match_result || !match_result->HasNext()) {
      logger::error("[DBManagerAdapter] Invalid SYNC build revision: {}", config.build_version);
      return DBManagerError::kInvalidSyncBuildVersion;
    }

    match_result = icon_data_mapper_->Match({config.sync_version, config.build_version, config.mode, "*", "*"});
    if (!match_result || !match_result->HasNext()) {
      logger::error("[DBManagerAdapter] Invalid HMI light mode: {}", HmiModeToString(config.mode));
      return DBManagerError::kInvalidCollectionMode;
    }

    logger::critical(
        "[DBManagerAdapter] CheckConfiguration(): failed to match config {},{},{} but could not determine which "
        "attribute is invalid.",
        config.sync_version, config.build_version, HmiModeToString(config.mode));
    return DBManagerError::kLogicError;
  }

  return DBManagerError::kSuccess;
}

DBManagerError DBManagerAdapter::Init(std::unique_ptr<db_manager::IconDataMapper> db_manager,
                                      const std::string& sync_version, const std::string& build_version,
                                      const std::string& mode) {
  icon_data_mapper_ = std::move(db_manager);
  assert(icon_data_mapper_);
  if (!icon_data_mapper_) {
    logger::critical("[DBManagerAdapter] Init(): icon_data_mapper_ was not created");
    return DBManagerError::kLogicError;
  }

  config_ = StorageConfig{sync_version, build_version, converters::ConfigStringToDbMode(mode)};
  return ValidateConfiguration(config_);
}

DBManagerError DBManagerAdapter::ChangeConfiguration(StorageConfig config) {
  auto error = ValidateConfiguration(config);
  if (error != DBManagerError::kSuccess) return error;

  error = CheckConfiguration(config);
  if (error != DBManagerError::kSuccess) return error;

  config_ = std::move(config);
  return DBManagerError::kSuccess;
}

DBManagerError DBManagerAdapter::ChangeSyncVersion(const std::string& sync_version, const std::string& build_version) {
  return ChangeConfiguration(StorageConfig{sync_version, build_version, config_.mode});
}

DBManagerError DBManagerAdapter::ChangeCollectionMode(const std::string& collection_mode) {
  return ChangeConfiguration(
      StorageConfig{config_.sync_version, config_.build_version, converters::ConfigStringToDbMode(collection_mode)});
}

cv::Mat DBManagerAdapter::GetItem(const std::string& name, const StorageConfig& config) {
  assert(icon_data_mapper_);
  if (!icon_data_mapper_) {
    logger::critical("[DBManagerAdapter] GetItem(): icon_data_mapper_ was not created");
    return {};
  }

  // Storage can change any time, so need to verify that selected config triplet <sync,build,mode> exists:
  if (CheckConfiguration(config) != DBManagerError::kSuccess) {
    logger::error(
        "[DBManagerAdapter] GetItem(): storage config '{}','{}','{}' is incompatible with current storage state",
        config.sync_version, config.build_version, HmiModeToString(config.mode));
    return {};
  }

  // Assemble identity as config triplet + name
  IconIdentity identity{config.sync_version, config.build_version, config.mode, name};

  // Try to fetch Icon from storage, and if such Icon does not exist - simply return empty mat
  try {
    auto pixmap = icon_data_mapper_->Get(identity).GetPixmap();

    if (!pixmap.empty()) {
      return cv::imdecode(cv::Mat(pixmap), cv::IMREAD_COLOR);
    } else {
      logger::error("[DBManagerAdapter] read empty pixmap for Icon ['{}', '{}', '{}', '{}'].", config.sync_version,
                    config.build_version, HmiModeToString(config.mode), name);
      return {};
    }
  } catch (const DbManagerError&) {
    return {};
  }
  return {};  // suppress compiler warning
}

cv::Mat DBManagerAdapter::GetItem(const std::string& name, const std::string& sync, const std::string& build,
                                  common::squish::CollectionMode mode) {
  StorageConfig config{sync, build, converters::SquishModeToDbMode(mode)};
  return this->GetItem(name, config);
}

cv::Mat DBManagerAdapter::GetItem(const std::string& name) { return this->GetItem(name, config_); }

std::vector<common::ObjectData> DBManagerAdapter::GetItemDataByWildcard(const common::ObjectDataIdentity& wildcard) {
  assert(icon_data_mapper_);
  if (!icon_data_mapper_) {
    logger::critical("[DBManagerAdapter] GetItemDataByWildcard(): icon_data_mapper_ was not created");
    return {};
  }

  std::vector<common::ObjectData> result;
  auto match_results = icon_data_mapper_->Match(converters::ObjectIdentityToDbWildcard(wildcard));
  if (match_results) {
    std::transform(match_results->begin(), match_results->end(), std::back_inserter(result),
                   converters::DbIconDataToObjectData);
  }

  return result;
}

std::vector<common::ObjectData> DBManagerAdapter::GetItemData(const std::string& object_name) {
  if (object_name.empty()) {  // it's workaround, because empty object name in wildcard means as asterisk
    return {};
  }

  common::ObjectDataIdentity identity{object_name, config_.sync_version, config_.build_version,
                                      converters::DbModeToSquishMode(config_.mode), "*"};

  return GetItemDataByWildcard(identity);
}
}  // namespace adapter
