#ifndef ATE_ERROR_DEFINES_H_
#define ATE_ERROR_DEFINES_H_

#include <stdexcept>

namespace squish {
class LookupError : public std::runtime_error {
 public:
  explicit LookupError() : std::runtime_error{"Failed to detect"} {}
};

class InvalidSyncVersion : public std::runtime_error {
 public:
  explicit InvalidSyncVersion() : std::runtime_error{"Invalid sync version"} {}
};

class InvalidSyncBuildVersion : public std::runtime_error {
 public:
  explicit InvalidSyncBuildVersion() : std::runtime_error{"Invalid sync build version"} {}
};

}  // namespace squish

#endif  // ATE_ERROR_DEFINES_H_
