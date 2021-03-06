#include "utils/screenshot_recorder.h"

#include <ctime>
#include <experimental/filesystem>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include "logger/logger.h"

namespace fs = std::experimental::filesystem;

namespace {
constexpr auto kExtension = ".png";
}  // namespace

namespace utils {

std::string GetStringTimestamp() {
  std::time_t t = std::time(nullptr);
  char buf[20];

  if (std::strftime(buf, sizeof(buf), "%F_%T", std::localtime(&t))) return buf;

  logger::warn("[screenshot_recorder] Can't convert timestamp to string");
  return std::string();
}

bool CheckDiskSpace(const cv::Mat& frame, std::error_code& ec) {
  constexpr const auto kCriticalLimitBytes = 300000000u;
  constexpr const auto kWarningLimitBytes = 500000000u;

  size_t bytes_to_write =
      frame.total() * frame.elemSize();  // this is not entirely correct, because written file on a disk is compressed

  fs::space_info root = fs::space("/", ec);
  if (ec) {
    logger::error("[screenshot recorder] {}", ec.message());
    return false;
  }

  bool result{true};

  if (bytes_to_write >= root.available || (root.available - bytes_to_write) <= kCriticalLimitBytes) {
    logger::error("[screenshot_recorder] Not enough disk space for writing: available {}b, requested to write {}b",
                  root.available, bytes_to_write);
    result = false;
  } else if ((root.available - bytes_to_write) <= kWarningLimitBytes) {
    logger::warn("[screenshot_recorder] Disk space close to critical: available {}b, requested to write {}b",
                 root.available, bytes_to_write);
  }

  return result;
}

ScreenshotRecorder::ScreenshotRecorder(bool enable_saving_screenshots, const std::string& screenshots_store_dir)
    : enable_saving_screenshots_(enable_saving_screenshots) {
  if (!enable_saving_screenshots) {
    throw std::runtime_error("Screenshot recorder is disabled");
  }

  screenshots_store_dir_ = ATE_WRITABLE_DATA_PREFIX;
  screenshots_store_dir_ /= screenshots_store_dir;

  std::error_code error = MakeDirectories(screenshots_store_dir_);

  if (error) {
    logger::warn("[screenshot_recorder] Can't create directory for storage screenshots at {}. {}",
                 screenshots_store_dir_, error.message());
    throw std::runtime_error(error.message());
  }
}

std::error_code ScreenshotRecorder::MakeDirectories(const fs::path& directory) {
  std::error_code ec;

  if (!fs::exists(directory, ec)) {
    if (ec != std::errc::no_such_file_or_directory) {
      logger::error("[screenshot_recorder] Failed to check directory existence {}", ec.message());
      return ec;
    }

    fs::create_directories(directory, ec);

    if (ec) {
      logger::error("[screenshot_recorder] Failed to create directory {}", ec.message());
      return ec;
    }
  }

  fs::permissions(directory,
                  fs::perms::owner_read | fs::perms::owner_write | fs::perms::owner_exec | fs::perms::group_read |
                      fs::perms::none | fs::perms::group_exec | fs::perms::others_read | fs::perms::others_exec,
                  ec);

  if (ec) {
    logger::error("[screenshot_recorder] Failed to set permissions {}", ec.message());
    return ec;
  }

  return std::error_code{};
}

fs::path ScreenshotRecorder::GetFileName(const std::string& file_suffix) const {
  std::string file_name = GetStringTimestamp();

  if (file_name.empty()) return fs::path();

  if (!file_suffix.empty()) {
    file_name += "_" + file_suffix;
  }
  file_name.append(".png");

  fs::path store_file = screenshots_store_dir_;
  store_file /= file_name;

  return store_file;
}

bool ScreenshotRecorder::SaveScreenshot(const cv::Mat& frame, const std::string& filename) const {
  std::error_code ec;
  if (!CheckDiskSpace(frame, ec)) return false;

  fs::path store_file = filename.empty() ? GetFileName() : (screenshots_store_dir_ / filename);

  if (store_file.empty()) return false;

  try {
    if (!cv::imwrite(store_file.c_str(), frame)) {
      logger::error("[screenshot_recorder] SaveScreenshot couldn't save file: '{}'", store_file.c_str());
      return false;
    }
  } catch (std::runtime_error& err) {
    logger::error("[screenshot_recorder] SaveScreenshot is failed: {}", err.what());
    return false;
  }

  logger::info("[screenshot_recorder] Screenshot was stored to {}", store_file);

  return true;
}

bool ScreenshotRecorder::SaveScreenshot(const cv::Mat& frame, const std::vector<cv::Rect>& highlight_areas,
                                        const std::string& hint) const {
  std::error_code ec;
  if (!CheckDiskSpace(frame, ec)) return false;

  std::string file_suffix = ((!highlight_areas.empty() && highlight_areas[0].area()) ? "detected" : "not-detected");

  if (!hint.empty()) file_suffix.append("_" + hint);

  fs::path store_file = GetFileName(file_suffix);

  if (store_file.empty()) return false;

  cv::Mat marked_image;
  cvtColor(frame, marked_image, cv::COLOR_GRAY2BGR);

  for (auto& item : highlight_areas) {
    rectangle(marked_image, item, cv::Scalar(0, 255, 0), 2, 8, 0);
  }

  if (!cv::imwrite(store_file.c_str(), marked_image)) {
    logger::error("[screenshot_recorder] SaveScreenshot couldn't save file: '{}'", store_file.c_str());
    return false;
  }

  logger::info("[screenshot_recorder] Screenshot was stored to {}", store_file);

  return true;
}

bool ScreenshotRecorder::TakeScreenshots(const cv::Mat& color_frame, const cv::Mat& grey_frame,
                                         const std::vector<cv::Rect>& highlight_areas, const std::string& hint) const {
  std::error_code ec;
  return CheckDiskSpace(color_frame, ec) && SaveScreenshot(color_frame) &&
         SaveScreenshot(grey_frame, highlight_areas, hint);
}

ScreenshotError ScreenshotRecorder::GetScreenshot(const cv::Mat& color_frame, const std::string& path,
                                                  const std::string& file_name) {
  std::error_code error;
  if (!CheckDiskSpace(color_frame, error)) {
    if (error) return ScreenshotError::kSystemError;
    return ScreenshotError::kNoAvailableDiskSpace;
  }

  if (file_name.empty() || fs::path(file_name).stem().empty()) {
    logger::error("[screenshot_recorder] Empty filename for screenshot");
    return ScreenshotError::kEmptyFileName;
  }

  fs::path store_file = ATE_WRITABLE_DATA_PREFIX;
  store_file /= path;
  store_file /= file_name;

  if (store_file.extension() != kExtension) {
    logger::error("[screenshot_recorder] Wrong extension for screenshot, please use {}", kExtension);
    return ScreenshotError::kWrongExtension;
  }

  fs::path parent_dir = store_file.parent_path();

  error = MakeDirectories(parent_dir);

  if (error) {
    if (error == std::errc::permission_denied || error == std::errc::operation_not_permitted) {
      return ScreenshotError::kPermissionDenied;
    } else {
      return ScreenshotError::kSystemError;
    }
  }

  try {
    if (!cv::imwrite(store_file.c_str(), color_frame)) {
      logger::error("[screenshot_recorder] GetScreenshot couldn't save file: '{}'", store_file.c_str());
      return ScreenshotError::kSystemError;
    }
  } catch (const std::runtime_error& exception) {
    logger::error("Exception converting image to PNG format: {}", exception.what());
    return ScreenshotError::kImageAssemblingFailed;
  }

  logger::info("[screenshot_recorder] Screenshot was stored to {}", store_file.string());
  return ScreenshotError::kSuccess;
}

}  // namespace utils
