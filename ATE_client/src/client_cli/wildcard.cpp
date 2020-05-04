#include "wildcard.h"

#include "converters.h"
#include "squish/wildcard.h"

namespace ATE {

using namespace impl_detail;

// clang-format off

System::Collections::Generic::List<ATE::Object^>^ Wildcard::GetMatchObjects() {
  auto w = ToSquishWildcard(this);
  auto result = gcnew System::Collections::Generic::List<ATE::Object^>();
  for (const auto& o : w.GetMatchObjects()) result->Add(ToAteObject(o));
  return result;
}

// clang-format on

}  // namespace ATE
