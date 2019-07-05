#ifndef ATE_SERVER_VIDEOSTREAMING_MATCHING_TEXT_DETECTOR_H_
#define ATE_SERVER_VIDEOSTREAMING_MATCHING_TEXT_DETECTOR_H_

#include <cstddef>
#include <iterator>
#include <memory>
#include <string>
#include <type_traits>

#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>

#include <tesseract/publictypes.h>

#include "detector.h"

namespace tesseract {
class ResultIterator;
class TessBaseAPI;
}  // namespace tesseract

namespace detector {

constexpr auto kDefaultLanguage = "eng";
constexpr auto kTessdataPrefixEnvVarName = "TESSDATA_PREFIX";
constexpr tesseract::PageIteratorLevel kDefaultComponentLevel = tesseract::RIL_BLOCK;

/**
 * @brief Represent text object on the image
 */
struct TextObject {
  /**
   * @brief Position of the text on image
   */
  cv::Rect position;

  /**
   * @brief Recognized text
   */
  std::string text;

  /**
   * @brief Confidence for the recognition in the range [0, 100]
   */
  float confidence = 0.f;
};

/**
 * @brief STL-style iterator for the TextDetectorRange
 */
class TextDetectorResultIterator {
 public:
  using value_type = TextObject;
  using difference_type = std::ptrdiff_t;
  using pointer = const value_type*;
  using reference = const value_type&;
  using iterator_category = std::input_iterator_tag;

  /**
   * @brief Construct a not dereferenceable TextDetectorResultIterator (e. g. end)
   */
  TextDetectorResultIterator() = default;

  /**
   * @brief Construct TextDetectorResultIterator
   *
   * @param tess - pointer to tesseract::TessBaseAPI
   * @param level - necessary granulation (e. g. cheracter, word, etc.)
   *
   * @throws std::invalid_argument if 'tess' is null or level isn't valid
   */
  TextDetectorResultIterator(std::shared_ptr<tesseract::TessBaseAPI> tess, tesseract::PageIteratorLevel level);

  /**
   * @return reference to the iterated object
   *
   * @note Behavior is undefined if this is not dereferenceable iterator
   */
  reference operator*() const noexcept;

  /**
   * @return pointer to the iterated object
   *
   * @note Behavior is undefined if this is not dereferenceable iterator
   */
  pointer operator->() const noexcept;

  /**
   * @brief Go to the next text object
   *
   * @return reference to this iterator
   *
   * @note
   * Make invalid all other shared copies of this iterator
   * Behavior is undefined if this is not dereferenceable iterator
   */
  TextDetectorResultIterator& operator++();

  friend bool operator==(const TextDetectorResultIterator& lhs, const TextDetectorResultIterator& rhs);
  friend bool operator!=(const TextDetectorResultIterator& lhs, const TextDetectorResultIterator& rhs);

 private:
  void ResetData();

  std::shared_ptr<tesseract::ResultIterator> tess_iter_;
  tesseract::PageIteratorLevel level_ = kDefaultComponentLevel;
  value_type data_;
};

static_assert(std::is_default_constructible<TextDetectorResultIterator>::value,
              "TextDetectorResultIterator must be default constructible");
static_assert(std::is_move_constructible<TextDetectorResultIterator>::value,
              "TextDetectorResultIterator must be Move constructible");
static_assert(std::is_move_assignable<TextDetectorResultIterator>::value,
              "TextDetectorResultIterator must be Move assignable");

/**
 * @brief Proxy class for TextDetectorResult with STL-style begin and end
 */
class TextDetectorResultRange {
 public:
  using const_iterator = TextDetectorResultIterator;
  using iterator = const_iterator;

  /**
   * @brief Construct TextDetectorResultRange
   *
   * @param tess - pointer to tesseract::TessBaseAPI
   * @param level - necessary granulation (e. g. cheracter, word, etc.)
   *
   * @throws std::invalid_argument if 'tess' is null
   */
  TextDetectorResultRange(std::shared_ptr<tesseract::TessBaseAPI> tess, tesseract::PageIteratorLevel level);

  const_iterator begin() const;
  const_iterator end() const;

 private:
  std::shared_ptr<tesseract::TessBaseAPI> tess_;
  tesseract::PageIteratorLevel level_ = kDefaultComponentLevel;
};

/**
 * @brief main class for text detection
 */
class TextDetector : public Detector<std::string> {
 public:
  /**
   * @param tessdata_prefix - value for TESSDATA_PREFIX environment variable
   * @param lang - language, default language is english (eng)
   *
   * @throws TextDetectorInitializationError if can't initialize
   */
  explicit TextDetector(const char* tessdata_prefix, const char* lang = "eng");

  /**
   * @brief Recognize text on the image
   *
   * @param image - image for text recognition
   *
   * @throws std::invalid_argument if
   * - 'image' is empty
   * - 'image' matrix isn't 2-dimensional
   * - 'image' isn't 1 or 3 channel with 8 bits per channel
   *
   * @return TextDetectorResult if successful
   * or null if 'image' is valid and recognition error occurs
   */
  bool Recognize(cv::InputArray image);

  /**
   * @brief Detect a text at a video frame
   * @param frame - video frame received from Sync (single channel matrix)
   * @param pattern - text pattern to be detected
   * @return pattern coordinates on succeed, otherwise return empty Rect
   **/
  cv::Rect Detect(const cv::Mat& frame, const std::string& pattern) const override;

  /**
   * @param level - necessary granulation (e. g. cheracter, word, etc.)
   *
   * @return TextDetectorResultRange proxy class with STL-style begin and end
   **/
  TextDetectorResultRange GetRange(tesseract::PageIteratorLevel level) const;

 private:
  std::shared_ptr<tesseract::TessBaseAPI> tess_;
};

}  // namespace detector

#endif  // ATE_SERVER_VIDEOSTREAMING_MATCHING_TEXT_DETECTOR_H_
