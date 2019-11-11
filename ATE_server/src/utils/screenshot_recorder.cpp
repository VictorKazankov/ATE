#include "utils/screenshot_recorder.h"

#include <ctime>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include "logger/logger.h"

namespace utils {

std::string GetStringTimestamp() {
  std::time_t t = std::time(nullptr);
  char buf[20];

  if (std::strftime(buf, sizeof(buf), "%F_%T", std::localtime(&t))) return buf;

  logger::warn("[screenshot_recorder] Can't convert timestamp to string");
  return std::string();
}

ScreenshotRecorder::ScreenshotRecorder(bool enable_saving_screenshots, const std::string& screenshots_store_dir)
    : enable_saving_screenshots_(enable_saving_screenshots) {
  if (!enable_saving_screenshots) {
    throw std::runtime_error("Screenshot recorder is disabled");
  }

  screenshots_store_dir_ = ATE_WRITABLE_DATA_PREFIX;
  screenshots_store_dir_ /= screenshots_store_dir;

  ProcessStorageDirectory();
}

void ScreenshotRecorder::ProcessStorageDirectory() {
  std::error_code ec;

  if (fs::exists(screenshots_store_dir_, ec)) {
  } else {
    if (fs::create_directories(screenshots_store_dir_, ec)) {
      logger::info("[screenshot_recorder] Directory for storage screenshots was created at {}", screenshots_store_dir_);
    } else {
      logger::warn("[screenshot_recorder] Can't create directory for storage screenshots at {}. {}",
                   screenshots_store_dir_, ec.message());
      throw std::runtime_error(ec.message());
    }
  }
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

bool ScreenshotRecorder::SaveScreenshot(const cv::Mat& frame) const {
  fs::path store_file = GetFileName();

  if (store_file.empty()) return false;

  cv::imwrite(store_file.c_str(), frame);

  logger::info("[screenshot_recorder] Screenshot was stored to {}", store_file);

  return true;
}

bool ScreenshotRecorder::SaveScreenshot(const cv::Mat& frame, const cv::Rect& highlight_area,
                                        const std::string& hint) const {
  std::string file_suffix = (highlight_area.area() ? "detected" : "not-detected");
  if (!hint.empty()) file_suffix.append("_" + hint);

  fs::path store_file = GetFileName(file_suffix);

  if (store_file.empty()) return false;

  cv::Mat marked_image;
  cvtColor(frame, marked_image, cv::COLOR_GRAY2BGR);

  rectangle(marked_image, highlight_area, cv::Scalar(0, 255, 0), 2, 8, 0);

  cv::imwrite(store_file.c_str(), marked_image);

  logger::info("[screenshot_recorder] Screenshot was stored to {}", store_file);

  return true;
}

bool ScreenshotRecorder::TakeScreenshots(const cv::Mat& color_frame, const cv::Mat& grey_frame,
                                         const cv::Rect& highlight_area, const std::string& hint) const {
  return SaveScreenshot(color_frame) && SaveScreenshot(grey_frame, highlight_area, hint);
}

}  // namespace utils
