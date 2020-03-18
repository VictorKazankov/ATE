#include "adapters/db_manager_adapter.h"

#include <algorithm>
#include <cassert>
#include <cctype>
#include <iterator>
#include <stdexcept>

#include <db_manager/access_credentials.h>
#include <db_manager/factory.h>

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

IconWildcard ObjectIdentityToDbWildcard(const common::ObjectDataIdentity& object_identity) {
  return IconWildcard{object_identity.sync_version, object_identity.build_version,
                      SquishModeToDbMode(object_identity.mode), object_identity.name, object_identity.parent_screen};
}

common::ObjectData DbIconToObjectData(const Icon& icon) {
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

AccessCredentials DBManagerAdapter::GetAccessCredentials() const {
  const std::string reader_login = "reader";
  const std::string reader_password = "JustCuriousWhatIsInside";
  const std::string database = "ate";
  const std::string table = "icon_storage";
  return AccessCredentials{reader_login, reader_password, database, table};
}

std::unique_ptr<IconDataMapper> DBManagerAdapter::CreateDataMapper(AccessCredentials access_credentials) const try {
  return CreateIconDataMapper(std::move(access_credentials));
} catch (const DbManagerError& err) {
  logger::error("[DBManagerAdapter] Can't create Icon Data Mapper: {}.", err.what());
  return {};
}

DBManagerAdapter::StorageConfig DBManagerAdapter::CreateConfiguration(std::string sync_version,
                                                                      std::string build_version,
                                                                      std::string mode) const {
  std::transform(mode.begin(), mode.end(), mode.begin(), [](unsigned char c) { return std::toupper(c); });
  if (mode == "DAY_MODE" || mode == "DAY") {
    return StorageConfig{std::move(sync_version), std::move(build_version), HmiMode::kDay};
  } else if (mode == "NIGHT_MODE" || mode == "NIGHT") {
    return StorageConfig{std::move(sync_version), std::move(build_version), HmiMode::kNight};
  } else {
    logger::critical("[DBManagerAdapter] Failed to parse MODE parameter. Defaulting to NONE mode.");
    return StorageConfig{std::move(sync_version), std::move(build_version), HmiMode::kNone};
  }
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
        "[DBManagerAdapter] CheckConfiguration(): failed to match config {},{},{} but could determine which attribute "
        "is invalid.",
        config.sync_version, config.build_version, HmiModeToString(config.mode));
    return DBManagerError::kLogicError;
  }

  return DBManagerError::kSuccess;
}

bool DBManagerAdapter::Init(const std::string&, const std::string& sync_version, const std::string& build_version,
                            const std::string& mode) {
  icon_data_mapper_ = CreateDataMapper(GetAccessCredentials());
  config_ = CreateConfiguration(sync_version, build_version, mode);
  return icon_data_mapper_ && CheckConfiguration(config_) == DBManagerError::kSuccess;
}

DBManagerError DBManagerAdapter::ChangeSyncVersion(const std::string& sync_version, const std::string& build_version) {
  auto new_config = CreateConfiguration(sync_version, build_version, HmiModeToString(config_.mode));
  DBManagerError error = CheckConfiguration(new_config);
  if (error == DBManagerError::kSuccess) config_ = std::move(new_config);
  return error;
}

DBManagerError DBManagerAdapter::ChangeCollectionMode(const std::string& collection_mode) {
  auto new_config = CreateConfiguration(config_.sync_version, config_.build_version, collection_mode);
  DBManagerError error = CheckConfiguration(new_config);
  if (error == DBManagerError::kSuccess) config_ = std::move(new_config);
  return error;
}

cv::Mat DBManagerAdapter::GetItem(const std::string& name) {
  assert(icon_data_mapper_);
  if (!icon_data_mapper_) {
    logger::critical("[DBManagerAdapter] GetItem(): icon_data_mapper_ was not created");
    return {};
  }

  IconIdentity identity{config_.sync_version, config_.build_version, config_.mode, name};

  // Try to fetch Icon from storage, and if such Icon does not exist - simply return empty mat
  try {
    auto pixmap = icon_data_mapper_->Get(identity).GetPixmap();

    if (!pixmap.empty()) {
      return cv::imdecode(cv::Mat(pixmap), cv::IMREAD_COLOR);
    } else {
      logger::error("[DBManagerAdapter] read empty pixmap for Icon [{}, {}, {}, {}].", config_.sync_version,
                    config_.build_version, HmiModeToString(config_.mode), name);
      return {};
    }
  } catch (const DbManagerError&) {
    return {};
  }
}

std::error_code DBManagerAdapter::ReloadStorage() noexcept {
  /* no-op

     With current adapted DB manager interface, reload operation does not need to be performed
  */
  return {};
}

std::vector<common::ObjectData> DBManagerAdapter::GetItemDataByWildcard(const common::ObjectDataIdentity& wildcard) {
  assert(icon_data_mapper_);
  if (!icon_data_mapper_) {
    logger::critical("[DBManagerAdapter] GetItemDataByWildcard(): icon_data_mapper_ was not created");
    return {};
  }

  using converters::DbIconToObjectData;
  using converters::ObjectIdentityToDbWildcard;

  std::vector<common::ObjectData> result;
  auto match_results = icon_data_mapper_->Match(ObjectIdentityToDbWildcard(wildcard));
  if (match_results) {
    // TODO:
    //   replace by mechanism that would not fetch Icon pixmap from storage as soon as it will be available in DbMgr
    std::transform(
        match_results->begin(), match_results->end(), std::back_inserter(result),
        [this](const IconIdentity& identity) { return DbIconToObjectData(icon_data_mapper_->Get(identity)); });
  }

  return result;
}

}  // namespace adapter
