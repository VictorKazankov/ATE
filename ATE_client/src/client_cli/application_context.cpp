#include "application_context.h"

#include "converters.h"

namespace ATE {

using namespace impl_detail;

// clang-format off

void ApplicationContext::Detach() {
  native_ref_.Detach();
}

System::String^ ApplicationContext::Host::get() {
   return ToSystemString(native_ref_.host());
}

System::String^ ApplicationContext::Port::get() {
  return ToSystemString(native_ref_.port());
}

// clang-format on

}  // namespace ATE
