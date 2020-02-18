#include "adapters/db_manager_adapter.h"

#include <db_manager/db_manager_error.h>
#include <db_manager/factory.h>

#include <opencv2/imgcodecs.hpp>

#include "logger/logger.h"

namespace adapter {

bool DBManagerAdapter::Init(const std::string& path_to_storage, const std::string& sync_version,
                            const std::string& sync_build_version, const std::string& collection_mode) {
  // Create storage manager
  std::error_code error;
  db_manager_ = db_manager::CreateFileStorageManager(path_to_storage, error);
  if (error) {
    logger::error("[DBManagerAdapter] Can't create storage manager: {} - {}.", error.message(), path_to_storage);
    return false;
  }

  // Save sync configuration
  sync_version_ = sync_version;
  sync_build_version_ = sync_build_version;
  collection_mode_ = collection_mode;

  // Check sync configuration and inform about incorrect value
  error = db_manager_->CheckConfiguration(sync_version_, sync_build_version_, collection_mode_);
  if (error) {
    if (error == db_manager::DBManagerError::kDontExistSyncVersion) {
      logger::warn(
          "Undefined sync version. Also invalid sync build version and collection mode for this sync version.");
    } else if (error == db_manager::DBManagerError::kDontExistSyncBuildVersion) {
      logger::warn("Undefined sync build version. Also invalid collection mode for this sync build version.");
    } else if (error == db_manager::DBManagerError::kDontExistCollectionMode) {
      logger::warn("Undefined collection mode.");
    }
  }

  return true;
}

DBManagerError DBManagerAdapter::ChangeSyncVersion(const std::string& sync_version,
                                                   const std::string& sync_build_version) {
  auto error = db_manager_->CheckConfiguration(sync_version, sync_build_version, collection_mode_);

  DBManagerError result{DBManagerError::kSuccess};
  if (!error) {
    // save new configuration
    sync_version_ = sync_version;
    sync_build_version_ = sync_build_version;
  } else {
    if (error == db_manager::DBManagerError::kDontExistSyncVersion) {
      result = DBManagerError::kInvalidSyncVersion;
    } else if (error == db_manager::DBManagerError::kDontExistSyncBuildVersion) {
      result = DBManagerError::kInvalidSyncBuildVersion;
    } else if (error == db_manager::DBManagerError::kDontExistCollectionMode) {
      // save new configuration
      sync_version_ = sync_version;
      sync_build_version_ = sync_build_version;
      result = DBManagerError::kInvalidCollectionMode;
    } else {
      logger::critical("[DBManagerAdapter::ChangeSyncVersion] Unhandled error.");
      assert(!"[DBManagerAdapter::ChangeSyncVersion] Unhandled error.");
    }
  }

  return result;
}

DBManagerError DBManagerAdapter::ChangeCollectionMode(const std::string& collection_mode) {
  auto error = db_manager_->CheckConfiguration(sync_version_, sync_build_version_, collection_mode);

  DBManagerError result{DBManagerError::kSuccess};
  if (!error) {
    // save new configuration
    collection_mode_ = collection_mode;
  } else {
    if (error == db_manager::DBManagerError::kDontExistSyncVersion) {
      result = DBManagerError::kInvalidSyncVersion;
    } else if (error == db_manager::DBManagerError::kDontExistSyncBuildVersion) {
      result = DBManagerError::kInvalidSyncBuildVersion;
    } else if (error == db_manager::DBManagerError::kDontExistCollectionMode) {
      result = DBManagerError::kInvalidCollectionMode;
    } else {
      logger::critical("[DBManagerAdapter::ChangeSyncVersion] Unhandled error.");
      assert(!"[DBManagerAdapter::ChangeSyncVersion] Unhandled error.");
    }
  }

  return result;
}

cv::Mat DBManagerAdapter::GetItem(const std::string& name) {
  const auto item_info = db_manager_->GetItem(name, sync_version_, sync_build_version_, collection_mode_);
  if (!item_info.item.data.empty()) {
    return cv::imdecode(cv::Mat(item_info.item.data), cv::IMREAD_COLOR);
  }

  return {};
}

std::error_code DBManagerAdapter::ReloadStorage() noexcept { return db_manager_->ReloadStorage(); }

std::vector<common::ObjectData> DBManagerAdapter::GetItemData(const std::string& pattern) {
  std::vector<common::ObjectData> object_data_list;
  // TODO(slisovenko@luxoft.com): temporary dummy. Replace this code after implementation select by pattern
  const auto item_info = db_manager_->GetItem(pattern, sync_version_, sync_build_version_, collection_mode_);
  if (item_info.item.IsValid()) {
    common::ObjectData object_data;
    object_data.center = item_info.item.center;
    object_data.top_left = item_info.item.top_left;
    object_data.bottom_right = item_info.item.bottom_right;
    object_data.parent_width = item_info.item.parent_width;
    object_data.parent_height = item_info.item.parent_height;
    object_data.name = item_info.item.alias;

    object_data_list.emplace_back(std::move(object_data));
  }

  return object_data_list;
}

}  // namespace adapter
