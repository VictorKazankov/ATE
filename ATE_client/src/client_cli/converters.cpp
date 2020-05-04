#include "converters.h"

#include <memory>
#include <typeinfo>

#include "enums.h"
#include "error_defines.h"
#include "errors.h"
#include "utils/squish_types.h"

namespace ATE {
namespace impl_detail {

// clang-format off

System::Exception^ ToSystemException(const std::exception& e) {
  if (typeid(e) == typeid(squish::LookupError))
    return gcnew ATE::LookupError();
  else if (typeid(e) == typeid(squish::InvalidSyncVersion))
    return gcnew ATE::InvalidSyncVersion();
  else if (typeid(e) == typeid(squish::InvalidSyncBuildVersion))
    return gcnew ATE::InvalidSyncBuildVersion();
  else if (typeid(e) == typeid(squish::InvalidSyncCollectionMode))
    return gcnew ATE::InvalidSyncCollectionMode();
  else if (typeid(e) == typeid(squish::InvalidDuration))
    return gcnew ATE::InvalidDuration();
  else if (typeid(e) == typeid(squish::EmptyScreenshotFileName))
    return gcnew ATE::EmptyScreenshotFileName();
  else if (typeid(e) == typeid(squish::WrongScreenshotExtension))
    return gcnew ATE::WrongScreenshotExtension();
  else if (typeid(e) == typeid(squish::PermissionDenied))
    return gcnew ATE::PermissionDenied();
  else if (typeid(e) == typeid(squish::ImageAssemblingFailed))
    return gcnew ATE::ImageAssemblingFailed();
  else if (typeid(e) == typeid(squish::InvalidRectangleCoordinates))
    return gcnew ATE::InvalidRectangleCoordinates();
  else if (typeid(e) == typeid(squish::ComparingImageIncorrectSize))
    return gcnew ATE::ComparingImageIncorrectSize();
  else if (typeid(e) == typeid(squish::ComparingImageNotExist))
    return gcnew ATE::ComparingImageNotExist();
  else if (typeid(e) == typeid(squish::UnsupportedFileType))
    return gcnew ATE::UnsupportedFileType();
  else if (typeid(e) == typeid(interaction::VideoStreamingError))
    return gcnew ATE::VideoStreamingError();
  else if (typeid(e) == typeid(interaction::NoConnectionEstablished))
    return gcnew ATE::NoConnectionEstablished();
  else if (typeid(e) == typeid(interaction::NoAvailableDiskSpace))
    return gcnew ATE::NoAvailableDiskSpace();
  else if (typeid(e) == typeid(std::invalid_argument))
    return gcnew System::ArgumentException(ToSystemString(e.what()));
  else
    return gcnew System::Exception(ToSystemString(e.what()));
}

std::string ToStdString(System::String^ s) {
  using namespace System;
  using namespace System::Runtime::InteropServices;

  auto deleter = [](char* p) { Marshal::FreeHGlobal(IntPtr{static_cast<void*>(p)}); };

  std::unique_ptr<char, decltype(deleter)> chars{
      static_cast<char*>(Marshal::StringToHGlobalAnsi(s).ToPointer()), deleter};
  return std::string{chars.get()};
}

System::String^ ToSystemString(const std::string& s) {
  return gcnew System::String(s.c_str());
}

squish::Object ToSquishObject(ATE::Object^ o) {
  squish::Object result{};
  result.x = o->X;
  result.y = o->Y;
  result.width = o->Width;
  result.height = o->Height;
  result.x_top_left = o->TopLeftX;
  result.y_top_left = o->TopLeftY;
  result.x_bottom_right = o->BottomRightX;
  result.y_bottom_right = o->BottomRightY;
  result.parent_width = o->ParentWidth;
  result.parent_height = o->ParentHeight;
  result.sync_version = ToStdString(o->SyncVersion);
  result.build_version = ToStdString(o->SyncBuildVersion);
  result.mode = static_cast<common::squish::CollectionMode>(o->SyncCollectionMode);
  result.name = ToStdString(o->Name);
  result.parent_screen = ToStdString(o->ParentScreen);
  return result;
}

ATE::Object^ ToAteObject(const squish::Object& o) {
  ATE::Object^ result = gcnew ATE::Object;
  result->X = o.x;
  result->Y = o.y;
  result->Width = o.width;
  result->Height = o.height;
  result->TopLeftX = o.x_top_left;
  result->TopLeftY = o.y_top_left;
  result->BottomRightX = o.x_bottom_right;
  result->BottomRightY = o.y_bottom_right;
  result->ParentWidth = o.parent_width;
  result->ParentHeight = o.parent_height;
  result->SyncVersion = ToSystemString(o.sync_version);
  result->SyncBuildVersion = ToSystemString(o.build_version);
  result->SyncCollectionMode = static_cast<CollectionMode>(o.mode);
  result->Name = ToSystemString(o.name);
  result->ParentScreen = ToSystemString(o.parent_screen);
  return result;
}

common::Point ToCommonPoint(ATE::ScreenPoint p) {
  return common::Point{p.X, p.Y};
}

common::Rect ToCommonRect(ATE::ScreenRectangle^ r) {
  return common::Rect{r->X, r->Y, r->Width, r->Height};
}

// clang-format on

}  // namespace impl_detail
}  // namespace ATE
