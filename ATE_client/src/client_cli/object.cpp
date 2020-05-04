#include "object.h"

#include <exception>

#include "api.h"
#include "converters.h"

namespace ATE {

using namespace impl_detail;

// clang-format off

bool Object::Exists(System::String^ objectName) try {
  return ::API::Exists(ToStdString(objectName));
} catch (const std::exception& e) {
  throw ToSystemException(e);
}

// clang-format on

}  // namespace ATE
