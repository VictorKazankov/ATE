#ifndef ATE_ERROR_DEFINES_H_
#define ATE_ERROR_DEFINES_H_

#include <stdexcept>

namespace squish {
class LookupError : public std::runtime_error {
 public:
  explicit LookupError() : std::runtime_error{"Failed to detect"} {}
};
}  // namespace squish

#endif  // ATE_ERROR_DEFINES_H_