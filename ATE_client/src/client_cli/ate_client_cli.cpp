#include "ate_client_cli.h"

#include <exception>

#include "api.h"
#include "converters.h"

namespace ATE {

using namespace System;
using namespace impl_detail;

// clang-format off

ApplicationContext^ API::AttachToApplication(String^ aut_name) try {
  return gcnew ApplicationContext(::API::AttachToApplication(ToStdString(aut_name)));
} catch (const std::exception& e) {
  throw gcnew Exception(ToSystemString(e.what()));
}

// clang-format on

}  // namespace ATE
