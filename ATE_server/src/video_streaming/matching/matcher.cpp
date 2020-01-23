#include "video_streaming/matching/matcher.h"

#include <cassert>
#include <utility>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "ate_error.h"
#include "factory/recognition_factory.h"
#include "logger/logger.h"
#include "utils/video_status.h"
#include "video_streaming/streamer.h"

namespace detector {

Matcher::Matcher(std::unique_ptr<streamer::Streamer> streamer, std::unique_ptr<Detector<cv::Mat>> image_detector,
                 std::unique_ptr<Detector<std::string>> text_detector,
                 std::unique_ptr<utils::ScreenshotRecorder> screenshot_recorder,
                 std::unique_ptr<utils::VideoStatus> video_status)
    : streamer_{std::move(streamer)},
      image_detector_{std::move(image_detector)},
      text_detector_{std::move(text_detector)},
      screenshot_recorder_{std::move(screenshot_recorder)},
      video_status_{std::move(video_status)} {
  assert(streamer_);
  assert(image_detector_);
  assert(text_detector_);
}

Matcher::~Matcher() = default;

bool Matcher::GrabNewFrame() {
  const bool result = streamer_->Frame(screen_);
  if (result) {
    cv::cvtColor(screen_, gray_screen_, cv::COLOR_BGR2GRAY);
  } else {
    // Clear old cache images
    screen_.release();
    gray_screen_.release();

    logger::critical("[matcher] Couldn't grab a frame");
  }
  return result;
}

std::pair<cv::Rect, std::error_code> Matcher::MatchImage(const std::string& object, const cv::Mat& pattern) {
  if (!video_status_->GetVideoStatus() || !GrabNewFrame()) {
    logger::error("[matcher] Video stream unavailable");
    return {cv::Rect{}, common::make_error_code(common::AteError::kVideoTemporarilyUnavailable)};
  }
  if (pattern.empty()) {
    logger::error("[matcher] Find pattern is empty");
    return {cv::Rect{}, common::make_error_code(common::AteError::kPatternInvalid)};
  }
  if (pattern.rows > screen_.rows || pattern.cols > screen_.cols) {
    logger::error("[matcher] Wrong pattern for image detection: pattern size: {}, screen size: {}", pattern.size,
                  screen_.size);

    return {cv::Rect{}, common::make_error_code(common::AteError::kPatternInvalid)};
  }

  const cv::Rect detected_object = image_detector_->Detect(screen_, pattern);

  if (screenshot_recorder_) {
    screenshot_recorder_->TakeScreenshots(screen_, gray_screen_, detected_object, object);
  }

  if (detected_object.empty()) {
    return {cv::Rect{}, common::make_error_code(common::AteError::kPatternNotFound)};
  }

  return {detected_object, std::error_code{}};
}

std::pair<cv::Rect, std::error_code> Matcher::MatchText(const std::string& text) {
  if (text.empty()) {
    logger::error("[matcher] The search text is empty");
    return {cv::Rect{}, common::make_error_code(common::AteError::kPatternInvalid)};
  }

  if (!video_status_->GetVideoStatus() || !GrabNewFrame()) {
    logger::error("[matcher] Video stream unavailable");
    return {cv::Rect{}, common::make_error_code(common::AteError::kVideoTemporarilyUnavailable)};
  }

  cv::Rect detected_area = text_detector_->Detect(screen_, text);

  if (screenshot_recorder_) {
    screenshot_recorder_->TakeScreenshots(screen_, gray_screen_, detected_area, text);
  }

  if (detected_area.empty()) {
    return {cv::Rect{}, common::make_error_code(common::AteError::kPatternNotFound)};
  }

  return {detected_area, std::error_code{}};
}

void Matcher::ChangeResolution(int x, int y) { streamer_->ChangeResolution(x, y); }

void Matcher::ChangePreprocessingList(const std::string& sync_version) {
  auto new_detector_instance = factory::CreateTextDetector(sync_version);
  if (new_detector_instance != nullptr) {
    text_detector_.swap(new_detector_instance);
  } else {
    logger::error("Can't create text detector for sync version: {}", sync_version);
  };
}

std::error_code Matcher::GetScreenshot(const std::string& file_name, const std::string& file_path) {
  if (!video_status_->GetVideoStatus() || !GrabNewFrame()) {
    logger::error("[matcher] Video stream unavailable");
    return {common::make_error_code(common::AteError::kVideoTemporarilyUnavailable)};
  }

  utils::ScreenshotError screenshot_error = utils::ScreenshotRecorder::GetScreenshot(screen_, file_name, file_path);

  if (screenshot_error != utils::ScreenshotError::kSuccess) {
    switch (screenshot_error) {
      case utils::ScreenshotError::kEmptyFileName:
        return {common::make_error_code(common::AteError::kEmptyFileName)};

      case utils::ScreenshotError::kWrongExtension:
        return {common::make_error_code(common::AteError::kWrongExtension)};

      case utils::ScreenshotError::kPermissionDenied:
        return {common::make_error_code(common::AteError::kPermissionDenied)};

      case utils::ScreenshotError::kImageAssemblingFailed:
        return {common::make_error_code(common::AteError::kImageAssemblingFailed)};

      default:
        return {common::make_error_code(common::AteError::kSystemError)};
    }
  }

  return std::error_code{};
}

std::pair<std::string, std::error_code> Matcher::GetText(const cv::Point& point, const cv::Point& delta_point) {
  if (!video_status_->GetVideoStatus() || !GrabNewFrame()) {
    logger::error("[matcher] Video stream unavailable");
    return {{}, common::make_error_code(common::AteError::kVideoTemporarilyUnavailable)};
  }

  cv::Rect screen_rect{0, 0, screen_.size().width, screen_.size().height};
  if (!screen_rect.contains(point) || !screen_rect.contains(delta_point)) {
    logger::error("[matcher] Desired area is out of screen boundaries");
    return {{}, common::make_error_code(common::AteError::kOutOfBoundaries)};
  }

  cv::Rect crop_area(point, delta_point);
  cv::Mat croped = screen_(crop_area);

  return {text_detector_->ExtractText(croped), std::error_code{}};
}

}  // namespace detector
