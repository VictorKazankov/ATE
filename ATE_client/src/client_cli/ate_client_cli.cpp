#include "ate_client_cli.h"

#include <exception>

#include "api.h"
#include "converters.h"

namespace ATE {

using namespace System;
using namespace impl_detail;

// clang-format off

ApplicationContext^ API::AttachToApplication(String^ autName) try {
  return gcnew ApplicationContext(::API::AttachToApplication(ToStdString(autName)));
} catch (const std::exception& e) {
  throw gcnew Exception(ToSystemString(e.what()));
}

ATE::Object^ API::WaitForObject(ATE::Object^ objectOrName) try {
  return ToAteObject(::API::WaitForObject(ToSquishObject(objectOrName)));
} catch (const squish::LookupError&) {
  throw gcnew LookupError();
} catch (const std::exception& e) {
  throw gcnew Exception(ToSystemString(e.what()));
}

ATE::Object^ API::WaitForObject(System::String^ objectOrName) try {
  return ToAteObject(::API::WaitForObject(ToStdString(objectOrName)));
} catch (const squish::LookupError&) {
  throw gcnew LookupError();
} catch (const std::exception& e) {
  throw gcnew Exception(ToSystemString(e.what()));
}

ATE::Object^ API::WaitForObject(ATE::Object^ objectOrName, int timeoutMSec) try {
  return ToAteObject(::API::WaitForObject(ToSquishObject(objectOrName), timeoutMSec));
} catch (const squish::LookupError&) {
  throw gcnew LookupError();
} catch(const std::exception& e) {
  throw gcnew Exception(ToSystemString(e.what()));
}

ATE::Object^ API::WaitForObject(System::String^ objectOrName, int timeoutMSec) try {
  return ToAteObject(::API::WaitForObject(ToStdString(objectOrName), timeoutMSec));
} catch (const squish::LookupError&) {
  throw gcnew LookupError();
} catch (const std::exception& e) {
  throw gcnew Exception(ToSystemString(e.what()));
}

  // clang-format on

}  // namespace ATE
