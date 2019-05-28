#ifndef ATE_COMMON_UTILS_FILE_DESCRIPTOR_H_
#define ATE_COMMON_UTILS_FILE_DESCRIPTOR_H_

#include <bitset>
#include <limits>
#include <string>

namespace utils {

class FileDescriptor {
 public:
  using FileOpenFlags = std::bitset<std::numeric_limits<unsigned>::digits>;

  ~FileDescriptor();
  FileDescriptor(const std::string& name, const FileOpenFlags flags);

  FileDescriptor(const FileDescriptor&) = delete;
  FileDescriptor& operator=(const FileDescriptor&) = delete;

  FileDescriptor(FileDescriptor&&) noexcept;
  FileDescriptor& operator=(FileDescriptor&&) noexcept;

  inline int Handle() const { return fd_; };

 private:
  int fd_;
};

}  // namespace utils

#endif  // ATE_COMMON_UTILS_FILE_DESCRIPTOR_H_
