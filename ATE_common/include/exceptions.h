#ifndef ATE_COMMON_EXCEPTIONS_H_
#define ATE_COMMON_EXCEPTIONS_H_

#include <stdexcept>
#include <system_error>

namespace config {
class FileReadFailure : public std::runtime_error {
 public:
  explicit FileReadFailure() : std::runtime_error{"Failed to read config file"} {}
  explicit FileReadFailure(const std::string& message)
      : std::runtime_error{std::string("Failed to read config file: ") + message} {}
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

namespace interaction {
class SpiOpenFailure : public std::system_error {
 public:
  explicit SpiOpenFailure() : std::system_error{std::make_error_code(std::errc::bad_file_descriptor)} {}
};

class SpiWriteFailure : public std::system_error {
 public:
  explicit SpiWriteFailure() : std::system_error{std::make_error_code(std::errc::io_error)} {}
};

class InteractionTypeError : public std::invalid_argument {
 public:
  explicit InteractionTypeError() : std::invalid_argument{"Undefined type of interaction"} {}
};
}  // namespace interaction

namespace streamer {
class InvalidConfig : public std::runtime_error {
 public:
  explicit InvalidConfig() : std::runtime_error{"Invalid video stream config"} {}
};

class StreamOpenFailure : public std::runtime_error {
 public:
  explicit StreamOpenFailure(const std::string& what) : std::runtime_error{what} {}
};
}  // namespace streamer

namespace detector {
class ImageDetectorTypeError : public std::runtime_error {
 public:
  explicit ImageDetectorTypeError() : std::runtime_error{"Undefined type of image matching"} {}
};
}  // namespace detector

#endif  // ATE_COMMON_EXCEPTIONS_H_
