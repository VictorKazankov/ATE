#include "converters_to_cli.h"

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

CollectionMode ToCli(common::squish::CollectionMode mode) {
  return static_cast<CollectionMode>(mode);
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
  result->SyncCollectionMode = ToCli(object.mode);
  result->Name = ToCli(object.name);
  result->ParentScreen = ToCli(object.parent_screen);
  return result;
}

System::Collections::Generic::List<ATE::Object^>^ ToCli(const std::vector<squish::Object>& objects) {
  auto result = gcnew System::Collections::Generic::List<ATE::Object^>();
  for (const auto& o : objects) result->Add(ToCli(o));
  return result;
}

System::Collections::Generic::List<System::String^>^ ToCli(const std::vector<std::string>& strings) {
  auto result = gcnew System::Collections::Generic::List<System::String^>();
  for (const auto& s : strings) result->Add(ToCli(s));
  return result;
}

// clang-format on

}  // namespace impl_detail
}  // namespace ATE
