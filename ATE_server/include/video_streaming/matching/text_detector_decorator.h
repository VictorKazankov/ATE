#ifndef VHAT_ATE_SERVER_INCLUDE_UTILS_TEXT_DETECTOR_DECORATOR_H_
#define VHAT_ATE_SERVER_INCLUDE_UTILS_TEXT_DETECTOR_DECORATOR_H_

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include <opencv2/core/mat.hpp>

#include "detector.h"

namespace detector {

/**
 * @brief Preprocessing function
 */
class FramePreprocessing {
 public:
  /**
   * @brief Binary frame preprocessing for better recognition
   * @param source Color frame for preprocessing
   * @param color_type color type for convert to gray scale
   * @param threshold_type type of threshold
   * @return Preprocessed frame
   */
  static cv::Mat BinaryPreprocessing(const cv::Mat& source, int color_type, int threshold_type);

  /**
   * @brief Frame resize for better recognition
   * @param source Frame for preprocessing
   * @param crop_coefficient crop
   * @return Preprocessed frame
   */
  static cv::Mat CropPreprocessing(const cv::Mat& source, double crop_coefficient);
};

/**
 * @brief An Decorator for text searcher that improve search
 */
class TextDetectorDecorator : public Detector<std::string> {
 private:
  /**
   * @brief An enum with frame preprocessing type
   */
  enum class ScreenPreprocessing {
    kUnknown = -1,
    kNone = 0,
    kBinarizedCrop = 1,
    kBinarized = 2,
    kBinarizedOTSU = 3,
    kBinarizedRGB = 4,  // TODO(SLisovenko@luxoft.com): need researching
  };

 public:
  explicit TextDetectorDecorator(std::unique_ptr<Detector<std::string>> text_detector,
                                 const std::string& preprocessing_lists);

  /**
   * @brief Detect text pattern
   * @param frame frame for search
   * @param pattern text for search
   * @return detected area
   */
  cv::Rect Detect(const cv::Mat& frame, const std::string& pattern) final;

 private:
  /**
   * @brief Correct detecting area for optimized frame
   * @param area Detected area for correcting
   * @param active_preprocessing Current preprocessing type
   */
  void CorrectionArea(cv::Rect& area, ScreenPreprocessing active_preprocessing) const noexcept;

  /**
   * @brief Fill array with preprocessing procedures
   * @param list_of_preprocessing_type array with active preprocessing procedure type
   */
  void FillPreprocessingList(const std::vector<ScreenPreprocessing>& list_of_preprocessing_type);

  /**
   * @brief Get description of optimization for logging
   * @param active_preprocessing Current preprocessing type
   * @return name of optimization
   */
  std::string GetCurrentPreprocessingDescription(ScreenPreprocessing active_preprocessing) const noexcept;

  /**
   * @brief Get preprocessing type by name
   * @param name preprocessing name
   * @return preprocessing type
   */
  ScreenPreprocessing GetScreenPreprocessingByName(const std::string& name) const noexcept;

  /**
   * @brief Get list of preprocessing
   * @param name preprocessing string ("None+BinarizedCrop+Binarized")
   * @return preprocessing list
   */
  std::vector<ScreenPreprocessing> GetPreprocessingList(const std::string& list_of_preprocessing) const noexcept;

 private:
  using PreprocessingFunctionType = std::function<cv::Mat(const cv::Mat&)>;
  using PreprocessingDataType = std::pair<ScreenPreprocessing, PreprocessingFunctionType>;

  const double crop_coefficient_{1.5};
  std::unique_ptr<Detector<std::string>> text_detector_;
  std::vector<PreprocessingDataType> preprocessing_lists_{};
};

}  // namespace detector

#endif  // VHAT_ATE_SERVER_INCLUDE_UTILS_TEXT_DETECTOR_DECORATOR_H_
