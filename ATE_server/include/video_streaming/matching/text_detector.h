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
   * @brief Tesseract result iterator
   */

  std::shared_ptr<tesseract::ResultIterator> tesseract_result_iterator;
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
   * @brief Construct TextDetectorResultIterator
   *
   * @param tess_result_itetator - pointer to tesseract::ResultIterator
   * @param level - necessary granulation (e. g. cheracter, word, etc.)
   *
   * @throws std::invalid_argument if 'tess_result_itetator' is null or level isn't valid
   */
  TextDetectorResultIterator(tesseract::ResultIterator* tess_result_itetator, tesseract::PageIteratorLevel level);

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
 * @brief class for text detection based on tesseract
 */
class TextDetector : public Detector<std::string> {
 private:
  using position_data = std::pair<size_t, TextObject>;
  using value_metadata = std::pair<std::string, position_data>;

 private:
  void SetImage(const cv::Mat& frame);

  tesseract::PageIteratorLevel GetPageLevel(const std::string& pattern) const;

  cv::Rect GetTextCoordinates(const std::string& pattern) const;

  /**
   * @brief Performs a simple search of pattern at the text, detected by tesseract
   * @return iterator with found data 
   */
  const TextDetectorResultIterator FindPattern(const tesseract::PageIteratorLevel& page_iterator_level,
                                               const std::string pattern) const;

  /**
   * @brief Looks for a sequence of words which should be found at the line
   * @param preprocessed_line - source line
   * @param pattern - pattern for search
   * @return rectangle with searched sequence
   */
  cv::Rect GetPhraseCoordinates(const std::vector<value_metadata>& preprocessed_line,
                                const std::vector<std::string>& pattern) const;

  /**
   * @brief PreprocessLine goes through the line and picks words from desired_words variable out with their metadata
   * (position, index at the line. etc.)
   * @param detector_result_iterator - line for looking desired_words at it
   * @param desired_words - words should be found
   * @return vector with metadata for searched words
   */
  std::vector<value_metadata> PreprocessLine(const std::shared_ptr<tesseract::ResultIterator>& detector_result_iterator,
                                             const std::vector<std::string>& desired_words) const;

 public:
  /**
   * @param text_detector_min_confidence - value for confidence level for recognized text
   * @param tessdata_prefix - value for TESSDATA_PREFIX environment variable
   * @param lang - language, default language is english (eng)
   *
   * @throws TextDetectorInitializationError if can't initialize
   */
  explicit TextDetector(double text_detector_min_confidence, const char* tessdata_prefix, const char* lang = "eng");

  /**
   * @brief Detect a text at a video frame
   * @param frame - video frame received from Sync (single channel matrix)
   * @param pattern - text pattern to be detected
   * @throws std::invalid_argument if
   * - 'text' is empty
   * - 'image' is empty
   * - 'image' matrix isn't 2-dimensional
   * - 'image' isn't 1 or 3 channel with 8 bits per channel
   * @return pattern coordinates on succeed, otherwise return empty Rect
   **/
  cv::Rect Detect(const cv::Mat& frame, const std::string& pattern) override;

 private:
  std::shared_ptr<tesseract::TessBaseAPI> tess_;
  double text_detector_min_confidence_;
};

}  // namespace detector

#endif  // ATE_SERVER_VIDEOSTREAMING_MATCHING_TEXT_DETECTOR_H_
