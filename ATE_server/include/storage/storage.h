#ifndef ATE_SERVER_STORAGE_STORAGE_H_
#define ATE_SERVER_STORAGE_STORAGE_H_

#include <string>

namespace storage {
/**
 * @brief Interface class for storage containers
 **/
class Storage {
 public:
  virtual void Connect(const std::string&) = 0;
  virtual std::string ItemPath(const std::string&) const = 0;

  virtual ~Storage() = default;
};
}  // namespace storage

#endif  // ATE_SERVER_STORAGE_STORAGE_H_
