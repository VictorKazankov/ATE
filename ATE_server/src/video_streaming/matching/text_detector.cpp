#include "video_streaming/matching/text_detector.h"

#include <cassert>
#include <utility>

#include <tesseract/baseapi.h>
#include <tesseract/resultiterator.h>

#include "logger/logger.h"
#include "utils/safe_env.h"
#include "video_streaming/matching/text_detector_exceptions.h"

namespace detector {

namespace {

constexpr auto kLoggingCategoryPrefix = "[text_detector] ";

[[noreturn]] void WriteErrorAndThrowInvalidArgument(const char* error_message) {
  logger::error("{}{}", kLoggingCategoryPrefix, error_message);
  throw std::invalid_argument{error_message};
}

}  // namespace

TextDetectorResultIterator::TextDetectorResultIterator(std::shared_ptr<tesseract::TessBaseAPI> tess,
                                                       tesseract::PageIteratorLevel level)
    : level_{level} {
  if (!tess) {
    WriteErrorAndThrowInvalidArgument("Null tesseract::TessBaseAPI pointer");
  }
  tesseract::ResultIterator* const unsafe_iter = tess->GetIterator();
  if (!unsafe_iter) {
    // Construct a not dereferenceable iterator
    logger::warn("{}tesseract::TessBaseAPI::GetIterator return a null pointer", kLoggingCategoryPrefix);
    return;
  }
  auto safe_deleter = [tess = std::move(tess)](tesseract::ResultIterator * iter) noexcept { delete iter; };
  std::unique_ptr<tesseract::ResultIterator, decltype(safe_deleter)> iter{unsafe_iter, std::move(safe_deleter)};
  if (iter->Empty(level)) {
    // Construct a not dereferenceable iterator
    return;
  }
  tess_iter_ = std::move(iter);
  ResetData();
}

TextDetectorResultIterator::reference TextDetectorResultIterator::operator*() const noexcept {
  assert(tess_iter_);
  return data_;
}

TextDetectorResultIterator::pointer TextDetectorResultIterator::operator->() const noexcept {
  assert(tess_iter_);
  return &data_;
}

TextDetectorResultIterator& TextDetectorResultIterator::operator++() {
  if (!tess_iter_) {
    assert(false);
    return *this;
  }
  if (tess_iter_->Next(level_)) {
    ResetData();
  } else {
    tess_iter_.reset();
  }
  return *this;
}

void TextDetectorResultIterator::ResetData() {
  cv::Point top_left;
  cv::Point bottom_right;

  tess_iter_->BoundingBox(level_, &top_left.x, &top_left.y, &bottom_right.x, &bottom_right.y);
  data_.position = cv::Rect{top_left, bottom_right};

  // Temporary unique_ptr is necessary
  // since the std::string constructor hasn't noexcept guarantee
  const std::unique_ptr<const char[]> raw_text{tess_iter_->GetUTF8Text(level_)};

  // Checking the null pointers is necessary
  // since the std::string constructor has undefined behavior in this case
  if (raw_text) {
    data_.text = raw_text.get();
  } else {
    data_.text.clear();
  }

  data_.confidence = tess_iter_->Confidence(level_);
}

bool operator==(const TextDetectorResultIterator& lhs, const TextDetectorResultIterator& rhs) {
  return (lhs.tess_iter_ && rhs.tess_iter_ && !lhs.tess_iter_->Cmp(*rhs.tess_iter_)) ||
         (!lhs.tess_iter_ && !rhs.tess_iter_);
}

bool operator!=(const TextDetectorResultIterator& lhs, const TextDetectorResultIterator& rhs) { return !(lhs == rhs); }

TextDetectorResultRange::TextDetectorResultRange(std::shared_ptr<tesseract::TessBaseAPI> tess,
                                                 tesseract::PageIteratorLevel level)
    : tess_{std::move(tess)}, level_{level} {
  if (!tess_) {
    WriteErrorAndThrowInvalidArgument("Null tesseract::TessBaseAPI pointer");
  }
}

TextDetectorResultRange::const_iterator TextDetectorResultRange::begin() const { return const_iterator{tess_, level_}; }

TextDetectorResultRange::const_iterator TextDetectorResultRange::end() const { return const_iterator{}; }

TextDetectorResultRange TextDetector::GetRange(tesseract::PageIteratorLevel level) const {
  return TextDetectorResultRange{tess_, level};
}

TextDetector::TextDetector(const char* tessdata_prefix, const char* lang) {
  safe_env::Set(kTessdataPrefixEnvVarName, tessdata_prefix, false);
  tess_ = std::make_shared<tesseract::TessBaseAPI>();
  if (tess_->Init(nullptr, lang)) {
    throw TextDetectorInitializationError{"Couldn't initialize text detector"};
  }
}

void TextDetector::SetImage(const cv::Mat& frame) {
  if (frame.empty()) {
    WriteErrorAndThrowInvalidArgument("Image must be non empty");
  }

  // Image matrix must be 2-dimensional
  if (2 != frame.dims) {
    WriteErrorAndThrowInvalidArgument("Image must be 2-dimensional");
  }

  if (CV_8UC1 != frame.type() && CV_8UC3 != frame.type()) {
    WriteErrorAndThrowInvalidArgument("Image must have 1 or 3 channels with 8 bits per channel");
  }

  tess_->SetImage(static_cast<const unsigned char*>(frame.data), frame.cols, frame.rows, frame.channels(),
                  frame.step1());
}

tesseract::PageIteratorLevel TextDetector::GetPageLevel(const std::string& text) const {
  if (1 == text.length()) {
    return tesseract::RIL_SYMBOL;
  }
  const auto check_space = [](char symbol) noexcept { return std::isspace(static_cast<unsigned char>(symbol)); };
  return std::any_of(text.begin(), text.end(), check_space) ? tesseract::RIL_TEXTLINE : tesseract::RIL_WORD;
}

cv::Rect TextDetector::Detect(const cv::Mat& frame, const std::string& pattern) {
  SetImage(frame);

  if (tess_->Recognize(nullptr)) {
    return cv::Rect();
  }

  const auto range = GetRange(GetPageLevel(pattern));

  const auto find_string = [ this, &pattern ](const TextObject& text_object) noexcept {
    return text_object.text == pattern;
  };

  const auto it = std::find_if(range.begin(), range.end(), find_string);
  return range.end() == it ? cv::Rect{} : it->position;
}
}  // namespace detector
