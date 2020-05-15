#include "wildcard.h"

#include <exception>

#include "converters_from_cli.h"
#include "converters_to_cli.h"
#include "squish/wildcard.h"

namespace ATE {

using namespace impl_detail;

// clang-format off

System::Collections::Generic::List<ATE::Object^>^ Wildcard::GetMatchObjects() try {
  auto w = FromCli(this);
  return ToCli(w.GetMatchObjects());
} catch (const std::exception& e) {
  throw Rethrow(e);
}

// clang-format on

}  // namespace ATE
