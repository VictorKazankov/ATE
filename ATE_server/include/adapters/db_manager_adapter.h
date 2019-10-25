#ifndef VHAT_DB_MANAGER_ADAPTER_H_
#define VHAT_DB_MANAGER_ADAPTER_H_

#include <memory>
#include <string>

#include <db_manager/db_manager.h>

namespace cv {
class Mat;
}

namespace adapter {

/**
 * @brief Proxy DBManager error
 */
enum class DBManagerError {
  kSuccess = 0,
  kInvalidSyncVersion,
  kInvalidSyncBuildVersion,
  kInvalidCollectionMode,
};

/**
 * @brief An adapter for storage manager
 */
class DBManagerAdapter {
 public:
  /**
   * @brief Storage initialization
   * @param path_to_storage Path to storage
   * @param sync_version Sync version
   * @param sync_build_version Sync build version
   * @param collection_mode Collection mode
   * @return True if initialization is success, false otherwise
   */
  bool Init(const std::string& path_to_storage, const std::string& sync_version, const std::string& sync_build_version,
            const std::string& collection_mode);

  /**
   * @brief Changes sync configuration
   * @param sync_version Sync version
   * @param sync_build_version Sync build version
   * @return Error code after change configuration
   */
  DBManagerError ChangeSyncVersion(const std::string& sync_version, const std::string& sync_build_version);

  /**
   * @brief Changes sync collection mode
   * @param collection_mode Collection mode
   * @return Error if incorrect collection_mode, otherwise success
   */
  adapter::DBManagerError ChangeCollectionMode(const std::string& collection_mode);

  /**
   * @brief Get item from collection
   * @param name Item name (key)
   * @return OpenCv Mat or empty Mat if item doesn't exist
   */
  cv::Mat GetItem(const std::string& name);

 private:
  db_manager::SyncVersion sync_version_;
  db_manager::SyncBuildVersion sync_build_version_;
  db_manager::CollectionMode collection_mode_;
  std::unique_ptr<db_manager::IDbManager> db_manager_;
};

}  // namespace adapter

#endif  // VHAT_DB_MANAGER_ADAPTER_H_
