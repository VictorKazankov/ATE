#include "video_streaming/matching/matcher.h"

#include <cassert>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "common.h"
#include "config/config.h"
#include "logger/logger.h"
#include "utils/defines.h"
#include "video_streaming/matching/template_detector.h"
#include "video_streaming/matching/text_detector.h"
#include "video_streaming/stream_reader.h"

namespace detector {

Matcher::Matcher()
    : streamer_{std::make_unique<streamer::StreamReader>(
          common::Config().GetString(defines::kBoardSection, defines::kVideoStreamOption, std::string{}))},
      image_detector_{std::make_unique<detector::TemplateDetector>()},
      text_detector_min_confidence_{common::Config().GetDouble(defines::kTextDetectorSection,
                                                               defines::kTextDetectorConfidenceOption,
                                                               defines::kDefaultTextDetectorConfidence)} {
  const std::string tessdata_prefix =
      common::Config().GetString(defines::kTextDetectorSection, defines::kTessDataOption, std::string{});
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

cv::Rect Matcher::MatchImage(const std::string& object_path) {
  if (!GrabNewFrame()) {
    return cv::Rect{};
  }
  const cv::Mat pattern = cv::imread(object_path, cv::IMREAD_GRAYSCALE);
  return image_detector_->Detect(gray_screen_, pattern);
}

cv::Rect Matcher::MatchText(const std::string& text) {
  if (!GrabNewFrame()) {
    return cv::Rect{};
  }
  const auto result = text_detector_->Recognize(screen_);
  if (!result) {
    return cv::Rect{};
  }
  auto range = result->GetRange(tesseract::RIL_TEXTLINE);

  const auto find_string = [this, &text](const TextObject& text_object) {
    return text_object.confidence >= text_detector_min_confidence_ && text_object.text == text;
  };
  auto it = std::find_if(range.begin(), range.end(), find_string);

  if (range.end() == it) {
    return cv::Rect{};
  }
  return it->position;
}

}  // namespace detector
