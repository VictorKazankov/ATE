#include "object.h"

#include <exception>

#include "api.h"
#include "converters_from_cli.h"

namespace ATE {

using namespace impl_detail;

// clang-format off

bool Object::Exists(System::String^ objectName) try {
  return ::API::Exists(FromCli(objectName), FromCli(ScreenPoint(0,0)), FromCli(ScreenPoint(0,0)));
} catch (const std::exception& e) {
  throw Rethrow(e);
}

bool Object::Exists(System::String^ objectName, ScreenPoint topLeft, ScreenPoint bottomRight) try {
  return ::API::Exists(FromCli(objectName), FromCli(topLeft), FromCli(bottomRight));
} catch (const std::exception& e) {
  throw Rethrow(e);
}

// clang-format on

}  // namespace ATE
