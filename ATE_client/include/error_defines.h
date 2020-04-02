#ifndef ATE_ERROR_DEFINES_H_
#define ATE_ERROR_DEFINES_H_

#include <stdexcept>

namespace squish {
class LookupError : public std::runtime_error {
 public:
  explicit LookupError() : std::runtime_error{"Failed to detect"} {}
};

class InvalidSyncVersion : public std::runtime_error {
 public:
  explicit InvalidSyncVersion() : std::runtime_error{"Invalid sync version"} {}
};

class InvalidSyncBuildVersion : public std::runtime_error {
 public:
  explicit InvalidSyncBuildVersion() : std::runtime_error{"Invalid sync build version"} {}
};

class InvalidSyncCollectionMode : public std::runtime_error {
 public:
  explicit InvalidSyncCollectionMode() : std::runtime_error{"Invalid sync collection mode"} {}
};

class InvalidDuration : public std::runtime_error {
 public:
  explicit InvalidDuration() : std::runtime_error{"Too long action duration"} {}
};

class EmptyScreenshotFileName : public std::runtime_error {
 public:
  explicit EmptyScreenshotFileName() : std::runtime_error{"The screenshot's filename is empty"} {}
};

class WrongScreenshotExtension : public std::runtime_error {
 public:
  explicit WrongScreenshotExtension() : std::runtime_error{"The screenshot has a wrong extension"} {}
};

class PermissionDenied : public std::runtime_error {
 public:
  explicit PermissionDenied() : std::runtime_error{"Saving file has been failed, permission denied"} {}
};

class ImageAssemblingFailed : public std::runtime_error {
 public:
  explicit ImageAssemblingFailed() : std::runtime_error{"Failed to assemble image"} {}
};

class InvalidRectangleCoordinates : public std::runtime_error {
 public:
  explicit InvalidRectangleCoordinates() : std::runtime_error("Invalid coordinates") {}
};

class ComparingImageIncorrectSize : public std::runtime_error {
 public:
  explicit ComparingImageIncorrectSize() : std::runtime_error("Comparing image is the incorrect size") {}
};

class ComparingImageNotExist : public std::runtime_error {
 public:
  explicit ComparingImageNotExist() : std::runtime_error("Comparing image not exist") {}
};

class UnsupportedFileType : public std::runtime_error {
 public:
  explicit UnsupportedFileType() : std::runtime_error("Unsupported file type") {}
};

}  // namespace squish

namespace interaction {
class VideoStreamingError : public std::runtime_error {
 public:
  explicit VideoStreamingError() : std::runtime_error{"Video stream not found"} {}
};

class NoConnectionEstablished : public std::runtime_error {
 public:
  explicit NoConnectionEstablished()
      : std::runtime_error{"No connection to server-side exists. First you need to perform attachToApplication API"} {}
};
}  // namespace interaction

#endif  // ATE_ERROR_DEFINES_H_
