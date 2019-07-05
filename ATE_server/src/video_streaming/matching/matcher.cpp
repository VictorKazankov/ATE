#include "video_streaming/matching/matcher.h"

#include <algorithm>
#include <cassert>
#include <cctype>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "common.h"
#include "config/config.h"
#include "logger/logger.h"
#include "utils/defines.h"
#include "video_streaming/matching/template_detector.h"
#include "video_streaming/matching/text_detector.h"
#include "video_streaming/streamer.h"
#include "video_streaming/streamer_factory.h"

namespace detector {

using namespace defines;

Matcher::Matcher()
    : streamer_{streamer::MakeStreamer()},
      image_detector_{std::make_unique<detector::TemplateDetector>(common::Config().GetDouble(
          kImageDetectorSection, kImageDetectorConfidenceOption, kDefaultImageDetectorConfidence))},
      text_detector_min_confidence_{common::Config().GetDouble(kTextDetectorSection, kTextDetectorConfidenceOption,
                                                               kDefaultTextDetectorConfidence)} {
  try {
    screenshot_recorder_ = std::make_unique<utils::ScreenshotRecorder>(
        common::Config().GetBool(kScreenshotRecorderSection, kScreenshotOption, false),
        common::Config().GetString(kScreenshotRecorderSection, kScreenshotDirectoryOption, ""));
  } catch (const std::runtime_error& e) {
    logger::info("[matcher] Screenshot recorder was not created, cause: {}.", e.what());
  }

  const std::string tessdata_prefix = common::Config().GetString(kTextDetectorSection, kTessDataOption, std::string{});
  if (tessdata_prefix.empty()) {
    logger::warn("[matcher] TESSDATA_PREFIX isn't present in config. Possible problems with text detection");
  }
  text_detector_ = std::make_unique<TextDetector>(tessdata_prefix.c_str());
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

cv::Rect Matcher::MatchImage(const std::string& object, const std::string& object_path) {
  if (!GrabNewFrame()) {
    return cv::Rect{};
  }

  const cv::Mat pattern = cv::imread(object_path, cv::IMREAD_GRAYSCALE);

  if (pattern.empty()) {
    logger::error("[matcher] The image cannot be read: {}", object_path);
    return cv::Rect{};
  }

  if (pattern.rows > gray_screen_.rows || pattern.cols > gray_screen_.cols) {
    logger::error("[matcher] Wrong pattern for image detection: pattern size: {}, screen size: {}", pattern.size,
                  gray_screen_.size);

    return cv::Rect{};
  }

  const cv::Rect detected_object = image_detector_->Detect(gray_screen_, pattern);

  if (screenshot_recorder_) {
    screenshot_recorder_->TakeScreenshots(screen_, gray_screen_, detected_object, object);
  }

  return detected_object;
}

cv::Rect Matcher::MatchText(const std::string& text) {
  if (text.empty() || !GrabNewFrame()) {
    return cv::Rect{};
  }

  cv::Rect detected_area = text_detector_->Detect(screen_, text);

  if (screenshot_recorder_) {
    screenshot_recorder_->TakeScreenshots(screen_, gray_screen_, detected_area, text);
  }

  return detected_area;
  
}
}  // namespace detector
