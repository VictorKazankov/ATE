#ifndef ATE_COMMON_EXCEPTIONS_H_
#define ATE_COMMON_EXCEPTIONS_H_

#include <stdexcept>

namespace config {
class FileReadFailure : public std::runtime_error {
 public:
  explicit FileReadFailure() : std::runtime_error{"Failed to read config file"} {}
};
}  // namespace config

namespace logger {
class ConfigFormatError : public std::runtime_error {
 public:
  explicit ConfigFormatError() : std::runtime_error{"Invalid LOG section in config file"} {}
};
}  // namespace logger

namespace storage {
class ConnectionFailure : public std::runtime_error {
 public:
  explicit ConnectionFailure() : std::runtime_error{"Failed connect to storage"} {}
};
}  // namespace storage

#endif  // ATE_COMMON_EXCEPTIONS_H_
