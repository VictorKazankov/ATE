#include "ate_error.h"

namespace {
struct AteErrorCategoryImpl : std::error_category {
  const char* name() const noexcept final;
  std::string message(int code) const final;
};

/**
 * @brief Return the name of category implemetation
 */
const char* AteErrorCategoryImpl::name() const noexcept { return "ATE"; }

/**
 * @brief get error message
 * @param generic error code which mapping to message
 * @return error message
 */
std::string AteErrorCategoryImpl::message(int code) const {
  switch (static_cast<common::AteError>(code)) {
    case common::AteError::kVideoTemporarilyUnavailable:
      return "Video output is temporary unavailable";

    case common::AteError::kPatternInvalid:
      return "Pattern is invalid";

    case common::AteError::kPatternNotFound:
      return "Pattern is not found";

      /*Do not add default case, every error code should have correspoding message*/
  }

  return "Unrecognized Error";
}

/**
 * @brief Create error category
 * @return error category
 */
const std::error_category& AteErrorCategory() {
  static const AteErrorCategoryImpl instance;
  return instance;
}

}  // namespace

namespace common {

/**
 * @brief Create std::error_code from custom (AteError) error code
 * @param error customer (AteError) enum
 * @return error_code
 */
std::error_code make_error_code(AteError error) { return {static_cast<int>(error), AteErrorCategory()}; }
}  // namespace common
