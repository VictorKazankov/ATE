#include "ate_client_cli.h"

#include "api.h"
#include "api_forwarder.h"

namespace ATE {

using namespace impl_detail;

// clang-format off

ApplicationContext^ API::AttachToApplication(System::String^ autName) {
  return InvokeRet(::API::AttachToApplication, autName);
}

ATE::Object^ API::WaitForObject(ATE::Object^ objectOrName) {
  return InvokeRet(kWaitForObjectForwarder, objectOrName);
}

ATE::Object^ API::WaitForObject(System::String^ objectOrName) {
  return InvokeRet(kWaitForObjectForwarder, objectOrName);
}

ATE::Object^ API::WaitForObject(ATE::Object^ objectOrName, int timeoutMSec) {
  return InvokeRet(kWaitForObjectForwarder, objectOrName, timeoutMSec);
}

ATE::Object^ API::WaitForObject(System::String^ objectOrName, int timeoutMSec) {
  return InvokeRet(kWaitForObjectForwarder, objectOrName, timeoutMSec);
}

void API::TapObject(ScreenPoint screenPoint, Modifier modifierState, MouseButton button) {
  InvokeVoid(kTapObjectForwarder, screenPoint, modifierState, button);
}

void API::TapObject(ScreenRectangle^ screenRectangle, Modifier modifierState, MouseButton button) {
  InvokeVoid(kTapObjectForwarder, screenRectangle, modifierState, button);
}

void API::TapObject(ATE::Object^ object, Modifier modifierState, MouseButton button) {
  InvokeVoid(kTapObjectForwarder, object, modifierState, button);
}

// clang-format on

}  // namespace ATE
