#ifndef ATE_DB_MANAGER_ADAPTER_H_
#define ATE_DB_MANAGER_ADAPTER_H_

#include <memory>
#include <string>

#include <db_manager/icon_data_mapper.h>

#include "utils/object_data_type.h"

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
  kLogicError,
};

/**
 * @brief An adapter for storage manager
 */
class DBManagerAdapter {
 public:
  /**
   * @brief Storage initialization
   * @param db_manager Database manager
   * @param path_to_storage Path to storage
   * @param sync_version Sync version
   * @param sync_build_version Sync build version
   * @param collection_mode Collection mode
   * @return Error code, where kSuccess means success
   */
  DBManagerError Init(std::unique_ptr<db_manager::IconDataMapper> db_manager, const std::string& sync_version,
                      const std::string& sync_build_version, const std::string& collection_mode);

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
   * @brief Get item from collection. The function uses stored config
   * @param name Item name (key)
   * @return OpenCv Mat or empty Mat if item doesn't exist
   */
  cv::Mat GetItem(const std::string& name);
  /**
   * @brief Get item from collection
   * @param name Item name (key)
   * @param sync Sync version
   * @param build Sync build
   * @param mode Collection mode
   * @return OpenCv Mat or empty Mat if item doesn't exist
   */
  cv::Mat GetItem(const std::string& name, const std::string& sync, const std::string& build,
                  common::squish::CollectionMode mode);

  /**
   * @brief Gets items data by wildcard
   * @param wildcard Pattern identity object
   * @return List of objects data
   */
  std::vector<common::ObjectData> GetItemDataByWildcard(const common::ObjectDataIdentity& wildcard);

  /**
   * @brief Gets item data by object name using initialized config params(sync, build, mode)
   * @param object_name Name of Object
   * @return List of objects data
   */
  std::vector<common::ObjectData> GetItemData(const std::string& object_name);

 private:
  struct StorageConfig {
    std::string sync_version;
    std::string build_version;
    db_manager::HmiMode mode{db_manager::HmiMode::kNone};
  };

  DBManagerError ValidateConfiguration(const StorageConfig& config) const;
  DBManagerError CheckConfiguration(const StorageConfig& config) const;
  DBManagerError ChangeConfiguration(StorageConfig config);

  /**
   * @brief Get item from collection
   * @param name Item name (key)
   * @param config Storage config: sync, build, mode
   * @return OpenCv Mat or empty Mat if item doesn't exist
   */
  cv::Mat GetItem(const std::string& name, const StorageConfig& config);

  StorageConfig config_;
  std::unique_ptr<db_manager::IconDataMapper> icon_data_mapper_;
};  // namespace adapter

}  // namespace adapter

#endif  // ATE_DB_MANAGER_ADAPTER_H_
