#include "ate_client_cli.h"

#include "api.h"
#include "api_forwarder.h"

namespace ATE {

using namespace impl_detail;

// clang-format off

namespace {
void CheckArg(System::Object^ arg, System::String^ arg_name) {
  if (!arg) throw gcnew System::ArgumentNullException(arg_name);
}
}  // namespace

ApplicationContext^ API::AttachToApplication(System::String^ autName) {
  CheckArg(autName, "autName");
  return InvokeRet(::API::AttachToApplication, autName);
}

ATE::Object^ API::WaitForObject(ATE::Object^ objectOrName) {
  CheckArg(objectOrName, "objectOrName");
  return InvokeRet(kWaitForObjectForwarder, objectOrName);
}

ATE::Object^ API::WaitForObject(System::String^ objectOrName) {
  CheckArg(objectOrName, "objectOrName");
  return InvokeRet(kWaitForObjectForwarder, objectOrName);
}

ATE::Object^ API::WaitForObject(ATE::Object^ objectOrName, int timeoutMSec) {
  CheckArg(objectOrName, "objectOrName");
  return InvokeRet(kWaitForObjectForwarder, objectOrName, timeoutMSec);
}

ATE::Object^ API::WaitForObject(System::String^ objectOrName, int timeoutMSec) {
  CheckArg(objectOrName, "objectOrName");
  return InvokeRet(kWaitForObjectForwarder, objectOrName, timeoutMSec);
}

void API::TapObject(ScreenPoint screenPoint, Modifier modifierState, MouseButton button) {
  InvokeVoid(kTapObjectForwarder, screenPoint, modifierState, button);
}

void API::TapObject(ScreenRectangle^ screenRectangle, Modifier modifierState, MouseButton button) {
  CheckArg(screenRectangle, "screenRectangle");
  InvokeVoid(kTapObjectForwarder, screenRectangle, modifierState, button);
}

void API::TapObject(ATE::Object^ object, Modifier modifierState, MouseButton button) {
  CheckArg(object, "object");
  InvokeVoid(kTapObjectForwarder, object, modifierState, button);
}

// clang-format on

}  // namespace ATE
