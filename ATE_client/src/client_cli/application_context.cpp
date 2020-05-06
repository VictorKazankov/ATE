#include "application_context.h"

#include "converters_to_cli.h"

namespace ATE {

using namespace impl_detail;

// clang-format off

void ApplicationContext::Detach() {
  native_ref_.Detach();
}

System::String^ ApplicationContext::Host::get() {
   return ToCli(native_ref_.host());
}

System::String^ ApplicationContext::Port::get() {
  return ToCli(native_ref_.port());
}

// clang-format on

}  // namespace ATE
