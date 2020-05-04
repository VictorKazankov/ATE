#include "ate_client_cli.h"

#include <exception>

#include "api.h"
#include "converters.h"
#include "utils/squish_types.h"

namespace ATE {

using namespace System;
using namespace impl_detail;

// clang-format off

ApplicationContext^ API::AttachToApplication(String^ autName) try {
  return gcnew ApplicationContext(::API::AttachToApplication(ToStdString(autName)));
} catch (const std::exception& e) {
  throw ToSystemException(e);
}

ATE::Object^ API::WaitForObject(ATE::Object^ objectOrName) try {
  return ToAteObject(::API::WaitForObject(ToSquishObject(objectOrName)));
} catch (const std::exception& e) {
  throw ToSystemException(e);
}

ATE::Object^ API::WaitForObject(System::String^ objectOrName) try {
  return ToAteObject(::API::WaitForObject(ToStdString(objectOrName)));
} catch (const std::exception& e) {
  throw ToSystemException(e);
}

ATE::Object^ API::WaitForObject(ATE::Object^ objectOrName, int timeoutMSec) try {
  return ToAteObject(::API::WaitForObject(ToSquishObject(objectOrName), timeoutMSec));
} catch (const std::exception& e) {
  throw ToSystemException(e);
}

ATE::Object^ API::WaitForObject(System::String^ objectOrName, int timeoutMSec) try {
  return ToAteObject(::API::WaitForObject(ToStdString(objectOrName), timeoutMSec));
} catch (const std::exception& e) {
  throw ToSystemException(e);
}

void API::TapObject(ScreenPoint screenPoint, Modifier modifierState, MouseButton button) try {
  ::API::TapObject(ToCommonPoint(screenPoint),
                   static_cast<common::squish::ModifierState>(modifierState),
                   static_cast<common::squish::MouseButton>(button));
} catch (const std::exception& e) {
  throw ToSystemException(e);
}

void API::TapObject(ScreenRectangle^ screenRectangle, Modifier modifierState, MouseButton button) try {
  ::API::TapObject(ToCommonRect(screenRectangle),
                   static_cast<common::squish::ModifierState>(modifierState),
                   static_cast<common::squish::MouseButton>(button));
} catch (const std::exception& e) {
  throw ToSystemException(e);
}

void API::TapObject(ATE::Object^ object, Modifier modifierState, MouseButton button) try {
  ::API::TapObject(ToSquishObject(object),
                   static_cast<common::squish::ModifierState>(modifierState),
                   static_cast<common::squish::MouseButton>(button));
} catch (const std::exception& e) {
  throw ToSystemException(e);
}

// clang-format on

}  // namespace ATE
