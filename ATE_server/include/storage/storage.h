#ifndef ATE_SERVER_STORAGE_STORAGE_H_
#define ATE_SERVER_STORAGE_STORAGE_H_

#include <string>

namespace storage {
/**
 * @brief Interface class for storage containers
 **/
class Storage {
 public:
  /**
   * @brief The function establish connect to the image storage
   * @return Returns true if successful, or false otherwise.
   **/
  virtual bool Connect() = 0;

  /**
   * @brief The function returns path to the image
   * @param Image name
   * @return Returns path to the image
   **/
  virtual std::string ItemPath(const std::string&) const = 0;
  
  /**
   * @brief The function changes image storage source
   * @param New collection name
   * @return Returns true if successful, or false otherwise.
   **/
  virtual bool ChangeCollection(const std::string&) = 0;

  virtual ~Storage() = default;
};
}  // namespace storage

#endif  // ATE_SERVER_STORAGE_STORAGE_H_
