#ifndef VHAT_ATE_SERVER_INCLUDE_UTILS_TEXT_DETECTOR_DECORATOR_H_
#define VHAT_ATE_SERVER_INCLUDE_UTILS_TEXT_DETECTOR_DECORATOR_H_

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include <opencv2/core/mat.hpp>

#include "text_detector.h"

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
   * @brief An enum with optimization type
   */
  enum class OptimizationType {
    kZero = 0,
    kSimple = 1,
    kAdvanced = 2,
  };

  /**
   * @brief An enum with frame preprocessing type
   */
  enum class ScreenPreprocessing {
    kNone = 0,
    kCrop = 1,
    kBinarized = 2,
    kOTSUBinarized = 3,
    kBinarizedRGB = 4,  // TODO(SLisovenko@luxoft.com): need researching
  };

  /**
   * @brief Log type message
   */
  enum class TypeMessage {
    kBegin,
    kFound,
    kNotFound,
  };

 public:
  explicit TextDetectorDecorator(std::unique_ptr<Detector<std::string>> text_detector, const std::string& sync_type,
                                 const std::string& optimization_name);

  /*!
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
   */
  void CorrectionArea(cv::Rect& area) const noexcept;

  /**
   * @brief Fill array with preprocessing procedures
   * @param optimization_type optimization type
   * @param sync_type Type of sync
   */
  void FillPreprocessingList(OptimizationType optimization_type, const std::string& sync_type);
  void FillPreprocessingListForSync3(OptimizationType optimization_type);
  void FillPreprocessingListForSync4(OptimizationType optimization_type);

  /*!
   * @brief Get name of optimization for logging
   * @return name of optimization
   */
  std::string GetCurrentPreprocessingName() const noexcept;

  /*!
   * @brief Get type of optimization by name
   * @param name name of type optimization
   * @return optimization type
   */
  OptimizationType GetOptimizationTypeByName(const std::string& name) const noexcept;

  /*!
   * @brief Logging current optimization processing
   * @param type type message for logging
   */
  void PrintLogMessage(TypeMessage type);

 private:
  using PreprocessingFunctionType = std::function<cv::Mat(const cv::Mat&)>;
  using PreprocessingDataType = std::pair<ScreenPreprocessing, PreprocessingFunctionType>;

  const double crop_coefficient_{1.5};
  mutable ScreenPreprocessing active_preprocessing_{ScreenPreprocessing::kNone};
  std::unique_ptr<Detector<std::string>> text_detector_;
  std::vector<PreprocessingDataType> preprocessing_lists_{};
};

}  // namespace detector

#endif  // VHAT_ATE_SERVER_INCLUDE_UTILS_TEXT_DETECTOR_DECORATOR_H_
