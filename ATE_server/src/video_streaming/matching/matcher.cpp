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

namespace {

tesseract::PageIteratorLevel GetPageLevel(const std::string& text) {
  assert(!text.empty());
  if (1 == text.length()) {
    return tesseract::RIL_SYMBOL;
  }
  const auto check_space = [](char symbol) noexcept { return std::isspace(static_cast<unsigned char>(symbol)); };
  return std::any_of(text.begin(), text.end(), check_space) ? tesseract::RIL_TEXTLINE : tesseract::RIL_WORD;
}

}  // namespace

namespace detector {

Matcher::Matcher()
    : streamer_{streamer::MakeStreamer(
          common::Config().GetString(defines::kVideoStreamSection, defines::kVideoStreamPathOption, {}))},
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

  if (pattern.empty()) {
    logger::error("[matcher] The image cannot be read: {}", object_path);
    return cv::Rect{};
  }

  if (pattern.rows > gray_screen_.rows || pattern.cols > gray_screen_.cols) {
    logger::error("Wrong pattern for image detection: pattern size: {}, screen size: {}", pattern.size,
                  gray_screen_.size);

    return cv::Rect{};
  }

  return image_detector_->Detect(gray_screen_, pattern);
}

cv::Rect Matcher::MatchText(const std::string& text) {
  if (text.empty() || !GrabNewFrame()) {
    return cv::Rect{};
  }

  if (!text_detector_->Recognize(screen_)) {
    return cv::Rect{};
  }

  const auto range = text_detector_->GetRange(GetPageLevel(text));

  const auto find_string = [ this, &text ](const TextObject& text_object) noexcept {
    return text_object.confidence >= text_detector_min_confidence_ && text_object.text == text;
  };

  const auto it = std::find_if(range.begin(), range.end(), find_string);
  return range.end() == it ? cv::Rect{} : it->position;
}

}  // namespace detector
