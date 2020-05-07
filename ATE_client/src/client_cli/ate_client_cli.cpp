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

ATE::Object^ API::WaitForObject(Wildcard^ wildcard) {
  CheckArg(wildcard, "wildcard");
  return InvokeRet(kWaitForObjectForwarder, wildcard);
}

ATE::Object^ API::WaitForObject(ATE::Object^ objectOrName, int timeoutMSec) {
  CheckArg(objectOrName, "objectOrName");
  return InvokeRet(kWaitForObjectForwarder, objectOrName, timeoutMSec);
}

ATE::Object^ API::WaitForObject(System::String^ objectOrName, int timeoutMSec) {
  CheckArg(objectOrName, "objectOrName");
  return InvokeRet(kWaitForObjectForwarder, objectOrName, timeoutMSec);
}

ATE::Object^ API::WaitForObject(Wildcard^ wildcard, int timeoutMSec) {
  CheckArg(wildcard, "wildcard");
  return InvokeRet(kWaitForObjectForwarder, wildcard, timeoutMSec);
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

void API::TouchAndDrag(System::String^ objectOrName, int x, int y, int dx, int dy, Modifier modifierState) {
  CheckArg(objectOrName, "objectOrName");
  InvokeVoid(kTouchAndDragForwarder, objectOrName, x, y, dx, dy, modifierState);
}

void API::TouchAndDrag(ATE::Object^ objectOrName, int x, int y, int dx, int dy, Modifier modifierState) {
  CheckArg(objectOrName, "objectOrName");
  InvokeVoid(kTouchAndDragForwarder, objectOrName, x, y, dx, dy, modifierState);
}

void API::PressAndHold(ScreenPoint screenPoint) {
  InvokeVoid(kPressAndHoldForwarder, screenPoint);
}

void API::PressAndHold(ScreenRectangle^ screenRectangle) {
  CheckArg(screenRectangle, "screenRectangle");
  InvokeVoid(kPressAndHoldForwarder, screenRectangle);
}

void API::PressAndHold(ATE::Object^ object) {
  CheckArg(object, "object");
  InvokeVoid(kPressAndHoldForwarder, object);
}

void API::PressRelease(ScreenPoint screenPoint) {
  InvokeVoid(kPressReleaseForwarder, screenPoint);
}

void API::PressRelease(ScreenRectangle^ screenRectangle) {
  CheckArg(screenRectangle, "screenRectangle");
  InvokeVoid(kPressReleaseForwarder, screenRectangle);
}

void API::PressRelease(ATE::Object^ object) {
  CheckArg(object, "object");
  InvokeVoid(kPressReleaseForwarder, object);
}

void API::ChangeSyncIconDB(System::String^ syncVersion, System::String^ syncBuildVersion) {
  CheckArg(syncVersion, "syncVersion");
  CheckArg(syncBuildVersion, "syncBuildVersion");
  InvokeVoid(::API::ChangeSyncIconDB, syncVersion, syncBuildVersion);
}

void API::ChangeSyncMode(CollectionMode collectionMode) {
  InvokeVoid(::API::ChangeSyncMode, collectionMode);
}

void API::LongPress(ATE::Object^ object, int timeoutMSec) {
  CheckArg(object, "object");
  InvokeVoid(kLongPressForwarder, object, timeoutMSec);
}

void API::LongPress(ATE::Object^ object, int x, int y, int timeoutMSec) {
  CheckArg(object, "object");
  InvokeVoid(kLongPressForwarder, object, x, y, timeoutMSec);
}

bool API::GetScreenshot(System::String^ fileName, System::String^ location) {
  CheckArg(fileName, "fileName");
  CheckArg(location, "location");
  return InvokeRet(::API::GetScreenshot, fileName, location);
}

System::String^ API::GetText(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
  return InvokeRet(::API::GetText, x1, y1, x2, y2);
}

System::Collections::Generic::List<ATE::Object^>^ API::GetObjectsDataByPattern(System::String^ name) {
  CheckArg(name, "name");
  return InvokeRet(kGetObjectsDataByPatternForwarder, name);
}

System::Collections::Generic::List<ATE::Object^>^ API::GetObjectsDataByPattern(Wildcard^ wildcard) {
  CheckArg(wildcard, "wildcard");
  return InvokeRet(kGetObjectsDataByPatternForwarder, wildcard);
}

int API::GetImagesDiscrepancy(System::String^ pathSecond, System::String^ pathFirst) {
  CheckArg(pathSecond, "pathSecond");
  CheckArg(pathFirst, "pathFirst");
  return InvokeRet(::API::GetImagesDiscrepancy, pathSecond, pathFirst, ScreenPoint(0, 0), ScreenPoint(0, 0));
}

int API::GetImagesDiscrepancy(System::String^ pathSecond, System::String^ pathFirst,
                              ScreenPoint topLeftCoordinate, ScreenPoint bottomRightCoordinate) {
  CheckArg(pathSecond, "pathSecond");
  CheckArg(pathFirst, "pathFirst");
  return InvokeRet(::API::GetImagesDiscrepancy, pathSecond, pathFirst, topLeftCoordinate, bottomRightCoordinate);
}

System::Collections::Generic::List<System::String^>^ API::CaptureFrames(int interval, int duration, ScreenPoint top_left,
                                                                        ScreenPoint bottom_right, System::String^ path) {
  CheckArg(path, "path");
  return InvokeRet(::API::CaptureFrames, interval, duration, top_left, bottom_right, path);
}

// clang-format on

}  // namespace ATE
