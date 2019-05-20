#include "utils/safe_env.h"

#include <stdlib.h>
#include <cerrno>
#include <mutex>
#include <system_error>

namespace {
std::mutex kEnvMutex;
}  // namespace

namespace safe_env {

std::pair<std::string, bool> Get(const char* name) {
  // Null pointer check is necessary to prevent SIGSEGV
  if (!name) {
    return std::make_pair(std::string{}, false);
  }
  const std::lock_guard<std::mutex> env_lock{kEnvMutex};
  const char* const val = ::secure_getenv(name);
  return val ? std::make_pair(std::string{val}, true) : std::make_pair(std::string{}, false);
}

void Set(const char* name, const char* value, bool overwrite) {
  // Null pointer check is necessary to prevent SIGSEGV
  if (!value) {
    throw std::system_error{std::make_error_code(std::errc::invalid_argument)};
  }
  const std::lock_guard<std::mutex> env_lock{kEnvMutex};
  if (::setenv(name, value, overwrite)) {
    throw std::system_error{errno, std::generic_category()};
  }
}

void Unset(const char* name) {
  const std::lock_guard<std::mutex> env_lock{kEnvMutex};
  if (::unsetenv(name)) {
    throw std::system_error{errno, std::generic_category()};
  }
}

}  // namespace safe_env
