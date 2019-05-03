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
    WriteErrorAndThrowInvalidArgument("Null Tesseract::TessBaseAPI pointer");
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

TextDetectorResult::TextDetectorResult(cv::InputArray image, const char* lang)
    : tess_{std::make_shared<tesseract::TessBaseAPI>()} {
  const cv::Mat matrix = image.getMat();

  if (matrix.empty()) {
    WriteErrorAndThrowInvalidArgument("Image must be non empty");
  }

  // Image matrix must be 2-dimensional
  if (2 != matrix.dims) {
    WriteErrorAndThrowInvalidArgument("Matrix must be 2-dimensional");
  }

  if (CV_8UC1 != matrix.type() && CV_8UC3 != matrix.type()) {
    WriteErrorAndThrowInvalidArgument("Image must have 1 or 3 channels with 8 bits per channel");
  }

  if (tess_->Init(nullptr, lang)) {
    throw TextDetectorInitializationError{"Couldn't initialize text detector"};
  }

  tess_->SetImage(static_cast<const unsigned char*>(matrix.data), matrix.cols, matrix.rows, matrix.channels(),
                  matrix.step1());

  if (tess_->Recognize(nullptr)) {
    throw TextDetectorRecognitionError{"the text detector didn't recognize the image"};
  }
}

TextDetectorResultRange TextDetectorResult::GetRange(tesseract::PageIteratorLevel level) const {
  return TextDetectorResultRange{tess_, level};
}

TextDetector::TextDetector(const char* tessdata_prefix) {
  safe_env::Set(kTessdataPrefixEnvVarName, tessdata_prefix, false);
}

std::unique_ptr<TextDetectorResult> TextDetector::Recognize(cv::InputArray image, const char* lang) const {
  try {
    return std::make_unique<TextDetectorResult>(image, lang);
  } catch (const TextDetectorRuntimeError& error) {
    logger::error("{}{}", kLoggingCategoryPrefix, error.what());
    return nullptr;
  }
}
}  // namespace detector
