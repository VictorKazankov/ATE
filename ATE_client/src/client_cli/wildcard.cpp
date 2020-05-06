#include "wildcard.h"

#include "converters_from_cli.h"
#include "converters_to_cli.h"
#include "squish/wildcard.h"

namespace ATE {

using namespace impl_detail;

// clang-format off

System::Collections::Generic::List<ATE::Object^>^ Wildcard::GetMatchObjects() {
  auto w = FromCli(this);
  auto result = gcnew System::Collections::Generic::List<ATE::Object^>();
  for (const auto& o : w.GetMatchObjects()) result->Add(ToCli(o));
  return result;
}

// clang-format on

}  // namespace ATE
