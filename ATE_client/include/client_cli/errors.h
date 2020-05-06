#ifndef ATE_CLIENT_CLIENT_CLI_ERRORS_H_
#define ATE_CLIENT_CLIENT_CLI_ERRORS_H_

#include <exception>

namespace ATE {

// clang-format off

namespace impl_detail {
/**
 * @brief Convert exception from std::exception family (both ATE and std) into System::Exception
 * @param e Exception to convert
 * @return Handle to exception type corresponding to type of e
 **/
System::Exception^ Rethrow(const std::exception& e);
}  // namespace impl_detail

/// <summary>Base class for all ATE specific exceptions</summary>
public ref class AteError : public System::Exception {};

/// <summary>Thrown if Object cannot be found</summary>
public ref class LookupError : public AteError {};

/// <summary>Thrown if selected SYNC product name is invalid</summary>
public ref class InvalidSyncVersion : public AteError {};

/// <summary>Thrown if selected SYNC build revision is invalid</summary>
public ref class InvalidSyncBuildVersion : public AteError {};

/// <summary>Thrown if selected HMI light mode is invalid</summary>
public ref class InvalidSyncCollectionMode : public AteError {};

/// <summary>Thrown if duration is invalid</summary>
public ref class InvalidDuration : public AteError {};

/// <summary>Thrown if screenshot file name is empty</summary>
public ref class EmptyScreenshotFileName : public AteError {};

/// <summary>Thrown if screenshot extension is invalid</summary>
public ref class WrongScreenshotExtension : public AteError {};

/// <summary>Thrown if permission denied</summary>
public ref class PermissionDenied : public AteError {};

/// <summary>Thrown if failed to save screenshot</summary>
public ref class ImageAssemblingFailed : public AteError {};

/// <summary>Thrown if top-left and bottom-right coordinates are mixed up or
/// rectangle has zero height/width or is out of frame boundaries</summary>
public ref class InvalidRectangleCoordinates : public AteError {};

/// <summary>Thrown if image size is incorrect</summary>
public ref class ComparingImageIncorrectSize : public AteError {};

/// <summary>Thrown if compared image does not exist</summary>
public ref class ComparingImageNotExist : public AteError {};

/// <summary>Thrown if image format is not supported (supported format are: 'png', 'jpeg', 'jpg')</summary>
public ref class UnsupportedFileType : public AteError {};

/// <summary>Thrown if video stream is not available</summary>
public ref class VideoStreamingError : public AteError {};

/// <summary>Thrown if not connected to ATE server</summary>
public ref class NoConnectionEstablished : public AteError {};

/// <summary>Thrown if out of disk space</summary>
public ref class NoAvailableDiskSpace : public AteError {};

// clang-format on

}  // namespace ATE

#endif  // ATE_CLIENT_CLIENT_CLI_ERRORS_H_
