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
  using IconMap = std::unordered_map<std::string, std::string>;

 public:
  JsonStorage(const std::string& storage_path, const std::string& collection_name);

  /**
   * @brief The function establish connect to the image storage
   * @return Returns true if successful, or false otherwise.
   **/
  bool Connect() override;

  /**
   * @brief The function returns path to the image
   * @param Image name
   * @return Returns path to the image
   **/
  std::string ItemPath(const std::string& icon_name) const override;

  /**
   * @brief The function changes image storage source
   * @param New collection name
   * @return Returns true if successful, or false otherwise.
   **/
  bool ChangeCollection(const std::string& collection_name);

  JsonStorage(const JsonStorage&) = delete;
  JsonStorage& operator=(const JsonStorage&) = delete;
  ~JsonStorage() = default;

 private:
  bool LoadCollection();

  std::string storage_path_;
  std::string collection_name_;
  IconMap collection_;
};
}  // namespace storage

#endif  // ATE_SERVER_STORAGE_JSON_STORAGE_H_
