#include "video_streaming/matching/text_detector.h"

#include <cassert>
#include <sstream>
#include <utility>
#include <vector>

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
  auto safe_deleter = [](tesseract::ResultIterator * iter) noexcept { delete iter; };
  std::unique_ptr<tesseract::ResultIterator, decltype(safe_deleter)> iter{unsafe_iter, std::move(safe_deleter)};
  if (iter->Empty(level)) {
    // Construct a not dereferenceable iterator
    return;
  }
  tess_iter_ = std::move(iter);
  ResetData();
}

TextDetectorResultIterator::TextDetectorResultIterator(tesseract::ResultIterator* tess_result_itetator,
                                                       tesseract::PageIteratorLevel level)
    : level_{level} {
  if (!tess_result_itetator) {
    WriteErrorAndThrowInvalidArgument("Null tesseract::TessBaseAPI pointer");
  }

  tess_iter_ = std::make_shared<tesseract::ResultIterator>(*tess_result_itetator);
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
  data_.tesseract_result_iterator = tess_iter_;

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

TextDetector::TextDetector(double text_detector_min_confidence, const char* tessdata_prefix, const char* lang)
    : text_detector_min_confidence_(text_detector_min_confidence) {
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

tesseract::PageIteratorLevel TextDetector::GetPageLevel(const std::string& pattern) const {
  if (1 == pattern.length()) {
    return tesseract::RIL_SYMBOL;
  }
  const auto check_space = [](char symbol) noexcept { return std::isspace(static_cast<unsigned char>(symbol)); };
  return std::any_of(pattern.begin(), pattern.end(), check_space) ? tesseract::RIL_TEXTLINE : tesseract::RIL_WORD;
}

cv::Rect TextDetector::GetTextCoordinates(const std::string& pattern) const {
  const auto page_iterator_level = GetPageLevel(pattern);

  const auto pattern_result_iterator = FindPattern(page_iterator_level, pattern);
  const auto text_detector_end_iterator = TextDetectorResultIterator{};

  // in case of looking for few words, first search the entrance in the whole line, then looking for desired words
  // coordinates
  if (page_iterator_level == tesseract::RIL_TEXTLINE && text_detector_end_iterator != pattern_result_iterator) {
    std::istringstream iss(pattern);
    std::vector<std::string> pattern_splitted_by_words{std::istream_iterator<std::string>{iss},
                                                       std::istream_iterator<std::string>{}};

    std::vector<value_metadata> preprocessed_line =
        PreprocessLine(pattern_result_iterator->tesseract_result_iterator, pattern_splitted_by_words);

    return GetPhraseCoordinates(preprocessed_line, pattern_splitted_by_words);
  }

  return text_detector_end_iterator == pattern_result_iterator ? cv::Rect{} : pattern_result_iterator->position;
}

const TextDetectorResultIterator TextDetector::FindPattern(const tesseract::PageIteratorLevel& page_iterator_level,
                                                           const std::string pattern) const {
  const auto find_string = [ this, &page_iterator_level, &pattern ](const TextObject& text_object) noexcept {
    switch (page_iterator_level) {
      case tesseract::RIL_SYMBOL:
      case tesseract::RIL_WORD:
        return text_object.confidence >= text_detector_min_confidence_ && text_object.text == pattern;

      case tesseract::RIL_TEXTLINE:
        return text_object.confidence >= text_detector_min_confidence_ &&
               text_object.text.find(pattern) != std::string::npos;

      default:
        logger::warn("{}Undefined page iterator level", kLoggingCategoryPrefix);
        break;
    }

    return false;
  };

  return std::find_if(TextDetectorResultIterator{tess_, page_iterator_level}, TextDetectorResultIterator{},
                      find_string);
}

std::vector<TextDetector::value_metadata> TextDetector::PreprocessLine(
    const std::shared_ptr<tesseract::ResultIterator>& detector_result_iterator,
    const std::vector<std::string>& desired_words) const {
  size_t ordinal_index{0};
  std::vector<value_metadata> found_result;

  const auto get_words_and_positions = [&found_result, &desired_words, &ordinal_index](const TextObject& text_object) {
    bool store_word;
    const auto last_it_in_sequence{desired_words.rbegin().base() - 1};

    for (auto it = desired_words.begin(); it != desired_words.end(); ++it) {
      store_word = text_object.text == *it ? true : false;

      if (!store_word && (it == desired_words.begin() || it == last_it_in_sequence)) {
        // at the first and last words could be missed beginning and end respectively
        std::size_t word_position = text_object.text.find(*it);
        // beginning of the first word was missed
        if (it == desired_words.begin() && word_position != 0 &&
            (word_position + (*it).length() == text_object.text.length())) {
          store_word = true;
        } else {
          // end of last word was missed
          if (it == last_it_in_sequence && word_position == 0) {
            store_word = true;
          }
        }
      }

      if (store_word) {
        logger::debug("{}Found pattern word '{}' with index position {}", kLoggingCategoryPrefix, *it, ordinal_index);
        found_result.emplace_back(std::make_pair(*it, std::make_pair(ordinal_index, text_object)));
      }
    }

    ++ordinal_index;
  };

  const auto sequence_end_iterator = TextDetectorResultIterator{};
  auto sequence_iterator = TextDetectorResultIterator{detector_result_iterator.get(), tesseract::RIL_WORD};

  std::for_each(sequence_iterator, sequence_end_iterator, get_words_and_positions);

  return found_result;
}  // namespace detector

cv::Rect TextDetector::GetPhraseCoordinates(const std::vector<value_metadata>& preprocessed_line,
                                            const std::vector<std::string>& pattern) const {
  cv::Rect first_position_at_sequence, last_position_at_sequence;
  int previous_word_index{-1};

  if (preprocessed_line.size() == pattern.size()) {
    // found words as much as need without duplicates
    first_position_at_sequence = preprocessed_line.begin()->second.second.position;
    last_position_at_sequence = preprocessed_line.rbegin()->second.second.position;
  } else {
    auto pattern_it = pattern.begin();

    for (auto results_it = preprocessed_line.begin(); results_it != preprocessed_line.end(); ++results_it) {
      size_t step_between_words = results_it->second.first - previous_word_index;
      bool is_words_are_neighboring = (previous_word_index >= 0) ? step_between_words == 1 : true;

      // a word found and it next to the previous one
      if (std::string::npos != results_it->second.second.text.find(*pattern_it) && is_words_are_neighboring) {
        // the first word in the search sequence
        if (pattern_it == pattern.begin()) {
          first_position_at_sequence = results_it->second.second.position;
          logger::debug("{}The first word of sequence '{}' index at line {}", kLoggingCategoryPrefix,
                        results_it->second.second.text, results_it->second.first);
        } else {
          // the last word in the search sequence
          if (*pattern_it == *pattern.rbegin()) {
            last_position_at_sequence = results_it->second.second.position;
            logger::debug("{}The last word of sequence '{}' index at line {}", kLoggingCategoryPrefix,
                          results_it->second.second.text, results_it->second.first);
            break;
          }
        }

        previous_word_index = results_it->second.first;
        ++pattern_it;
      } else {
        if (!first_position_at_sequence.empty()) {
          // two identical words in a row next to each other, changing out first found word with that
          if (results_it->second.second.text == *pattern.begin()) {
            first_position_at_sequence = results_it->second.second.position;
            previous_word_index = results_it->second.first;
          } else {
            // nothing was found, so settings are resetting
            logger::debug("{}Reset found words settings", kLoggingCategoryPrefix);
            previous_word_index = -1;
            pattern_it = pattern.begin();
            first_position_at_sequence.x = first_position_at_sequence.y = first_position_at_sequence.width =
                first_position_at_sequence.height = 0;
          }
        }
      }
    }
  }

  // rectangle created from the left top corner of first word at the sequence and right bottom of the last one
  return cv::Rect(cv::Point(first_position_at_sequence.x, first_position_at_sequence.y),
                  cv::Point(last_position_at_sequence.x + last_position_at_sequence.width,
                            last_position_at_sequence.y + last_position_at_sequence.height));
}  // namespace detector

cv::Rect TextDetector::Detect(const cv::Mat& frame, const std::string& pattern) {
  if (pattern.empty()) {
    const std::string msg = "Nothing to detect, pattern is empty";
    logger::warn("{}{}", kLoggingCategoryPrefix, msg);
    throw std::invalid_argument{msg};
  }

  SetImage(frame);

  if (tess_->Recognize(nullptr)) {
    return cv::Rect();
  }

  return GetTextCoordinates(pattern);
}
}  // namespace detector
