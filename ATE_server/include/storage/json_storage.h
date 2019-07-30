#ifndef ATE_SERVER_STORAGE_JSON_STORAGE_H_
#define ATE_SERVER_STORAGE_JSON_STORAGE_H_

#include <string>
#include <unordered_map>

#include "storage/storage.h"

namespace storage {
/**
 * @brief Scheme of file database discribe at <https://adc.luxoft.com/jira/browse/VHAT-229>
 * This class repressent interaction with file database.
 **/
class JsonStorage : public Storage {
  using IconMap = std::unordered_map<std::string, fs::path>;

 public:
  JsonStorage(const fs::path& storage_path);

  /**
   * @brief The function returns path to the image
   * @param Image name
   * @return Returns path to the image on succeed, otherwise empty string
   **/
  fs::path ItemPath(const std::string& icon_name) const override;

  /**
   * @brief Loads image storage source if possible otherwise does nothing
   * @param New collection type
   * @param New collection build version
   * @param New collection name
   * @return Returns true if successful, or false otherwise.
   **/
  bool LoadCollection(const std::string& collection_type, const std::string& collection_build,
                      const std::string& collection_name) override;

  JsonStorage(const JsonStorage&) = delete;
  JsonStorage& operator=(const JsonStorage&) = delete;
  ~JsonStorage() override = default;

 private:
  fs::path storage_path_;
  std::string collection_type_;
  std::string collection_build_;
  std::string collection_name_;
  IconMap collection_;
};
}  // namespace storage

#endif  // ATE_SERVER_STORAGE_JSON_STORAGE_H_
