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

cv::Mat DBManagerAdapter::GetItem(const std::string& name) {
  const auto item_info = db_manager_->GetItem(name, sync_version_, sync_build_version_, collection_mode_);
  if (!item_info.item.data.empty()) {
    return cv::imdecode(cv::Mat(item_info.item.data), cv::IMREAD_COLOR);
  }

  return {};
}

}  // namespace adapter
