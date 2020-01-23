#ifndef ATE_ERROR_H_
#define ATE_ERROR_H_

#include <system_error>

namespace common {

/**
 * @brief ATE Server error code. Used to determine the result of function.
 */
enum class AteError {
  kVideoTemporarilyUnavailable = 101,
  kPatternInvalid,
  kPatternNotFound,
  kInvalidDurationLongPress,
  kEmptyFileName,
  kPermissionDenied,
  kWrongExtension,
  kSystemError,
  kImageAssemblingFailed,
  kOutOfBoundaries
};

/**
 * @brief Create error code from AteError
 * @param error AteError
 * @return std::error_code
 */
std::error_code make_error_code(AteError);

}  // namespace common

namespace std {
template <>
struct is_error_code_enum<common::AteError> : true_type {};
}  // namespace std

#endif
