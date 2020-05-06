#include "object.h"

#include <exception>

#include "api.h"
#include "converters_from_cli.h"

namespace ATE {

using namespace impl_detail;

// clang-format off

bool Object::Exists(System::String^ objectName) try {
  return ::API::Exists(FromCli(objectName));
} catch (const std::exception& e) {
  throw Rethrow(e);
}

// clang-format on

}  // namespace ATE
