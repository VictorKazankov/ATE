#include "errors.h"

#include <typeinfo>

#include "converters_to_cli.h"
#include "error_defines.h"

namespace ATE {
namespace impl_detail {

// clang-format off

System::Exception^ Rethrow(const std::exception& e) {
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
    return gcnew System::ArgumentException(ToCli(e.what()));
  else
    return gcnew System::Exception(ToCli(e.what()));
}

// clang-format on

}  // namespace impl_detail
}  // namespace ATE
