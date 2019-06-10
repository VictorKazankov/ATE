#ifndef ATE_SERVER_STORAGE_STORAGE_H_
#define ATE_SERVER_STORAGE_STORAGE_H_

#include <experimental/filesystem>
#include <string>

namespace fs = std::experimental::filesystem;

namespace storage {
/**
 * @brief Interface class for storage containers
 **/
class Storage {
 public:
  /**
   * @brief The function returns path to the image
   * @param Image name
   * @return Returns path to the image
   **/
  virtual fs::path ItemPath(const std::string&) const = 0;

  /**
   * @brief Loads image storage source if possible otherwise does nothing
   * @param New collection name
   * @return Returns true if successful, or false otherwise.
   **/
  virtual bool LoadCollection(const std::string&) = 0;

  virtual ~Storage() = default;
};
}  // namespace storage

#endif  // ATE_SERVER_STORAGE_STORAGE_H_
