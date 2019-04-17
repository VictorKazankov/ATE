#ifndef ATE_SERVER_STORAGE_JSON_STORAGE_H_
#define ATE_SERVER_STORAGE_JSON_STORAGE_H_

#include <string>
#include <unordered_map>

#include "storage/storage.h"

namespace storage {
/**
 * @brief Scheme of file database discribe at <https://adc.luxoft.com/jira/browse/VHAT-229>
 * This class repressent interaction with file database.
 * - storage_name_ - contain path to json files with styles
 * - collection_name_ - name of json file which contain mapping between icon name and path to icon
 * - collection_ - map which contain icon name and path to icon
 **/
class JsonStorage : public Storage {
 public:
  using IconMap = std::unordered_map<std::string, std::string>;

  JsonStorage(const std::string& storage_name, const std::string& collection_name);

  void Connect(const std::string& storage_name) override;
  std::string ItemPath(const std::string& icon_name) const override;
  void ChangeCollection(const std::string& collection_name);
  IconMap Collection() const;

  JsonStorage(const JsonStorage&) = delete;
  JsonStorage& operator=(const JsonStorage&) = delete;
  ~JsonStorage() = default;

 private:
  void LoadCollection();

  std::string storage_name_;
  std::string collection_name_;
  IconMap collection_;
};
}  // namespace storage

#endif  // ATE_SERVER_STORAGE_JSON_STORAGE_H_
