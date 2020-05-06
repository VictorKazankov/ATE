#include "ate_client_cli.h"

#include <exception>

#include "api.h"
#include "converters_from_cli.h"
#include "converters_to_cli.h"

namespace ATE {

using namespace System;
using namespace impl_detail;

// clang-format off

ApplicationContext^ API::AttachToApplication(String^ autName) try {
  return ToCli(::API::AttachToApplication(FromCli(autName)));
} catch (const std::exception& e) {
  throw Rethrow(e);
}

ATE::Object^ API::WaitForObject(ATE::Object^ objectOrName) try {
  return ToCli(::API::WaitForObject(FromCli(objectOrName)));
} catch (const std::exception& e) {
  throw Rethrow(e);
}

ATE::Object^ API::WaitForObject(System::String^ objectOrName) try {
  return ToCli(::API::WaitForObject(FromCli(objectOrName)));
} catch (const std::exception& e) {
  throw Rethrow(e);
}

ATE::Object^ API::WaitForObject(ATE::Object^ objectOrName, int timeoutMSec) try {
  return ToCli(::API::WaitForObject(FromCli(objectOrName), timeoutMSec));
} catch (const std::exception& e) {
  throw Rethrow(e);
}

ATE::Object^ API::WaitForObject(System::String^ objectOrName, int timeoutMSec) try {
  return ToCli(::API::WaitForObject(FromCli(objectOrName), timeoutMSec));
} catch (const std::exception& e) {
  throw Rethrow(e);
}

void API::TapObject(ScreenPoint screenPoint, Modifier modifierState, MouseButton button) try {
  ::API::TapObject(FromCli(screenPoint),
                   FromCli(modifierState),
                   FromCli(button));
} catch (const std::exception& e) {
  throw Rethrow(e);
}

void API::TapObject(ScreenRectangle^ screenRectangle, Modifier modifierState, MouseButton button) try {
  ::API::TapObject(FromCli(screenRectangle),
                   FromCli(modifierState),
                   FromCli(button));
} catch (const std::exception& e) {
  throw Rethrow(e);
}

void API::TapObject(ATE::Object^ object, Modifier modifierState, MouseButton button) try {
  ::API::TapObject(FromCli(object),
                   FromCli(modifierState),
                   FromCli(button));
} catch (const std::exception& e) {
  throw Rethrow(e);
}

// clang-format on

}  // namespace ATE
