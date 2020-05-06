#include "converters_from_cli.h"

#include <memory>

#include "enums.h"
#include "utils/squish_types.h"

namespace ATE {
namespace impl_detail {

// clang-format off

std::string FromCli(System::String^ cli_str) {
  using namespace System;
  using namespace System::Runtime::InteropServices;

  auto deleter = [](char* p) { Marshal::FreeHGlobal(IntPtr{static_cast<void*>(p)}); };

  std::unique_ptr<char, decltype(deleter)> chars{
      static_cast<char*>(Marshal::StringToHGlobalAnsi(cli_str).ToPointer()), deleter};
  return std::string{chars.get()};
}

squish::Object FromCli(ATE::Object^ cli_object) {
  squish::Object result{};
  result.x = cli_object->X;
  result.y = cli_object->Y;
  result.width = cli_object->Width;
  result.height = cli_object->Height;
  result.x_top_left = cli_object->TopLeftX;
  result.y_top_left = cli_object->TopLeftY;
  result.x_bottom_right = cli_object->BottomRightX;
  result.y_bottom_right = cli_object->BottomRightY;
  result.parent_width = cli_object->ParentWidth;
  result.parent_height = cli_object->ParentHeight;
  result.sync_version = FromCli(cli_object->SyncVersion);
  result.build_version = FromCli(cli_object->SyncBuildVersion);
  result.mode = static_cast<common::squish::CollectionMode>(cli_object->SyncCollectionMode);
  result.name = FromCli(cli_object->Name);
  result.parent_screen = FromCli(cli_object->ParentScreen);
  return result;
}

squish::Wildcard FromCli(ATE::Wildcard^ cli_wildcard) {
  return squish::Wildcard{FromCli(cli_wildcard->Name),
                          FromCli(cli_wildcard->SyncVersion),
                          FromCli(cli_wildcard->SyncBuildVersion),
                          FromCli(cli_wildcard->ParentScreen),
                          static_cast<common::squish::CollectionMode>(cli_wildcard->SyncCollectionMode)};
}

common::Point FromCli(ATE::ScreenPoint cli_point) {
  return common::Point{cli_point.X, cli_point.Y};
}

common::Rect FromCli(ATE::ScreenRectangle^ cli_rect) {
  return common::Rect{cli_rect->X, cli_rect->Y, cli_rect->Width, cli_rect->Height};
}

// clang-format on

}  // namespace impl_detail
}  // namespace ATE
