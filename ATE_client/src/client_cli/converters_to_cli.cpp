#include "converters_to_cli.h"

#include "enums.h"
#include "utils/squish_types.h"

namespace ATE {
namespace impl_detail {

// clang-format off

System::String^ ToCli(const char* native_str) {
  return gcnew System::String(native_str);
}

System::String^ ToCli(const std::string& native_str) {
  return ToCli(native_str.c_str());
}

ApplicationContext^ ToCli(squish::ApplicationContext& app_context) {
  return gcnew ApplicationContext(app_context);
}

ATE::Object^ ToCli(const squish::Object& object) {
  ATE::Object^ result = gcnew ATE::Object;
  result->X = object.x;
  result->Y = object.y;
  result->Width = object.width;
  result->Height = object.height;
  result->TopLeftX = object.x_top_left;
  result->TopLeftY = object.y_top_left;
  result->BottomRightX = object.x_bottom_right;
  result->BottomRightY = object.y_bottom_right;
  result->ParentWidth = object.parent_width;
  result->ParentHeight = object.parent_height;
  result->SyncVersion = ToCli(object.sync_version);
  result->SyncBuildVersion = ToCli(object.build_version);
  result->SyncCollectionMode = static_cast<CollectionMode>(object.mode);
  result->Name = ToCli(object.name);
  result->ParentScreen = ToCli(object.parent_screen);
  return result;
}

// clang-format on

}  // namespace impl_detail
}  // namespace ATE
