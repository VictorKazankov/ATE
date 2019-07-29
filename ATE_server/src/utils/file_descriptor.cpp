#include "utils/file_descriptor.h"

#include <fcntl.h>
#include <unistd.h>
#include <cerrno>
#include <memory>
#include <system_error>

#include "logger/logger.h"

namespace utils {

FileDescriptor::~FileDescriptor() {
  if (fd_ >= 0) {
    if (::close(fd_) != 0) logger::error("[utils][file descriptor: ~FileDescriptor()] Failed to close file {}: {}", fd_, errno);
  }
}

FileDescriptor::FileDescriptor(const std::string& name, const FileOpenFlags flags) : fd_{::open(name.c_str(), flags.to_ulong())} {
  if (fd_ < 0) throw std::system_error{errno, std::generic_category(), "Failed to open file: " + name};
}

FileDescriptor::FileDescriptor(FileDescriptor&& rhs) noexcept { *this = std::move(rhs); }

FileDescriptor& FileDescriptor::operator=(FileDescriptor&& rhs) noexcept {
  // close current descriptor before moving
  if (fd_ >= 0) {
    if (::close(fd_) != 0) logger::error("[utils][file descriptor: operator=(FileDescriptor&&)] Failed to close file {}: {}", fd_, errno);
  }
  // move
  fd_ = rhs.fd_;
  rhs.fd_ = -1;
  return *this;
}

}  // namespace utils
