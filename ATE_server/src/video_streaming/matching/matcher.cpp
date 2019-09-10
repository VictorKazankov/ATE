#include "video_streaming/matching/matcher.h"

#include <cassert>
#include <utility>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>

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

cv::Rect Matcher::MatchImage(const std::string& object, const cv::Mat& pattern) {
  if (!GrabNewFrame()) {
    return cv::Rect{};
  }

  if (pattern.empty()) {
    logger::error("[matcher] Find pattern is empty");
    return cv::Rect{};
  }

  if (pattern.rows > screen_.rows || pattern.cols > screen_.cols) {
    logger::error("[matcher] Wrong pattern for image detection: pattern size: {}, screen size: {}", pattern.size,
                  screen_.size);

    return cv::Rect{};
  }

  const cv::Rect detected_object = image_detector_->Detect(screen_, pattern);

  if (screenshot_recorder_) {
    screenshot_recorder_->TakeScreenshots(screen_, gray_screen_, detected_object, object);
  }

  return detected_object;
}

cv::Rect Matcher::MatchText(const std::string& text) {
  if (text.empty() || !GrabNewFrame()) {
    logger::error("[matcher] The search text is empty");
    return cv::Rect{};
  }

  cv::Rect detected_area = text_detector_->Detect(screen_, text);

  if (screenshot_recorder_) {
    screenshot_recorder_->TakeScreenshots(screen_, gray_screen_, detected_area, text);
  }

  return detected_area;
}
}  // namespace detector
