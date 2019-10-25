#include "video_streaming/matching/matcher.h"

#include <cassert>
#include <utility>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "ate_error.h"
#include "logger/logger.h"
#include "video_streaming/streamer.h"

namespace detector {

Matcher::Matcher(std::unique_ptr<streamer::Streamer> streamer, std::unique_ptr<Detector<cv::Mat>> image_detector,
                 std::unique_ptr<Detector<std::string>> text_detector,
                 std::unique_ptr<utils::ScreenshotRecorder> screenshot_recorder)
    : streamer_{std::move(streamer)},
      image_detector_{std::move(image_detector)},
      text_detector_{std::move(text_detector)},
      screenshot_recorder_{std::move(screenshot_recorder)} {
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
  if (!IsVideoStreamAvailable() || !GrabNewFrame()) {
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

  if (!IsVideoStreamAvailable() || !GrabNewFrame()) {
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

bool Matcher::IsVideoStreamAvailable() {
  // TODO (nttu): Implement video status checking function via gpio
  // Dummy implementation
  return true;
}

}  // namespace detector
