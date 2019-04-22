#ifndef ATE_SERVER_CONFIG_EXCEPTIONS_H_
#define ATE_SERVER_CONFIG_EXCEPTIONS_H_

#include <stdexcept>

namespace config {

class ConfigFileReadFailure : public std::runtime_error {
 public:
  explicit ConfigFileReadFailure() : std::runtime_error{"Failed to read config file."} {}
};

}  // namespace config

#endif  // ATE_SERVER_CONFIG_EXCEPTIONS_H_
