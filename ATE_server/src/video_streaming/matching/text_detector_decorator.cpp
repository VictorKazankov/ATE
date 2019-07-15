#include "video_streaming/matching/text_detector_decorator.h"

#include <map>

#include <opencv2/imgproc.hpp>

#include "logger/logger.h"

namespace detector {

const std::string kSync3 = "sync3";
const std::string kSync4 = "sync4";

cv::Mat FramePreprocessing::BinaryPreprocessing(const cv::Mat& source, int color_type, int threshold_type) {
  cv::Mat optimized_screen{};
  cv::Mat gray_screen{};
  cv::cvtColor(source, gray_screen, color_type);
  // convert to binary format
  cv::threshold(gray_screen, optimized_screen, 127, 255, threshold_type);
  return optimized_screen;
}

cv::Mat FramePreprocessing::CropPreprocessing(const cv::Mat& source, double crop_coefficient) {
  auto resized_screen = cv::Mat{};
  // image resizing
  auto crop_colls = static_cast<int>(source.cols / crop_coefficient);
  auto crop_rows = static_cast<int>(source.rows / crop_coefficient);
  cv::resize(source, resized_screen, cv::Size(crop_colls, crop_rows));

  return resized_screen;
}

TextDetectorDecorator::TextDetectorDecorator(std::unique_ptr<Detector<std::string>> text_detector,
                                             const std::string& sync_type, const std::string& optimization_name)
    : text_detector_(std::move(text_detector)) {
  // runtime error if sync type is unknown
  if (sync_type != kSync3 && sync_type != kSync4) {
    throw std::runtime_error("Undefined sync configuration");
  }
  logger::trace("[TextDetectorDecorator] Init for {}", sync_type);

  // runtime error if detector is nullptr
  if (text_detector_ == nullptr) {
    logger::critical("[TextDetectorDecorator] Text detector not initialized!");
    throw std::runtime_error("Text detector is nullptr");
  }

  // fill preprocessing steps
  FillPreprocessingList(GetOptimizationTypeByName(optimization_name), sync_type);
}

cv::Rect TextDetectorDecorator::Detect(const cv::Mat& frame, const std::string& pattern) {
  auto detected_area = cv::Rect{};

  if (pattern.empty()) {
    logger::warn("[TextDetectorDecorator] Nothing to detect, pattern is empty");
    return detected_area;
  }
  if (frame.empty()) {
    logger::warn("[TextDetectorDecorator] Nothing to detect, frame is empty");
    return detected_area;
  }

  // find pattern with preprocessing
  if (frame.channels() > 1) {
    for (const auto& preprocessing_iter : preprocessing_lists_) {
      if (preprocessing_iter.first == ScreenPreprocessing::kNone || preprocessing_iter.second == nullptr) {
        active_preprocessing_ = ScreenPreprocessing::kNone;
        PrintLogMessage(TypeMessage::kBegin);
        detected_area = text_detector_->Detect(frame, pattern);
      } else {
        // set optimization
        active_preprocessing_ = preprocessing_iter.first;
        PrintLogMessage(TypeMessage::kBegin);
        detected_area = text_detector_->Detect(preprocessing_iter.second(frame), pattern);
      }
      // if find object - break out from loop
      if (!detected_area.empty()) {
        CorrectionArea(detected_area);
        PrintLogMessage(TypeMessage::kFound);
        break;
      }
      PrintLogMessage(TypeMessage::kNotFound);
    }
  }
  // if grayscale mode - find without preprocessing
  else {
    logger::trace("[TextDetectorDecorator] Find without preprocessing. Frame is grayscale.");
    detected_area = text_detector_->Detect(frame, pattern);
  }

  return detected_area;
}

void TextDetectorDecorator::CorrectionArea(cv::Rect& area) const noexcept {
  if (active_preprocessing_ == ScreenPreprocessing::kCrop) {
    area.x = static_cast<int>(area.x * crop_coefficient_);
    area.y = static_cast<int>(area.y * crop_coefficient_);
  }
}

void TextDetectorDecorator::FillPreprocessingList(OptimizationType optimization_type, const std::string& sync_type) {
  if (sync_type == kSync4) {
    FillPreprocessingListForSync4(optimization_type);
  } else if (sync_type == kSync3) {
    FillPreprocessingListForSync3(optimization_type);
  }
}

void TextDetectorDecorator::FillPreprocessingListForSync3(TextDetectorDecorator::OptimizationType optimization_type) {
  // Preprocessing functions
  auto empty = [](const cv::Mat& source) { return source; };
  auto binary_preprocessing = [](const cv::Mat& source) {
    return FramePreprocessing::BinaryPreprocessing(source, cv::COLOR_BGR2GRAY, CV_THRESH_BINARY | CV_THRESH_OTSU);
  };

  switch (optimization_type) {
    case OptimizationType::kZero: {
      preprocessing_lists_.emplace_back(ScreenPreprocessing::kNone, empty);
      break;
    }
    case OptimizationType::kSimple:
    case OptimizationType::kAdvanced: {
      preprocessing_lists_.emplace_back(ScreenPreprocessing::kNone, empty);
      preprocessing_lists_.emplace_back(ScreenPreprocessing::kOTSUBinarized, binary_preprocessing);
      break;
    }
  }
}

void TextDetectorDecorator::FillPreprocessingListForSync4(TextDetectorDecorator::OptimizationType optimization_type) {
  // Preprocessing functions
  auto empty = [](const cv::Mat& source) { return source; };
  auto crop_preprocessing = [crop = this->crop_coefficient_](const cv::Mat& source) {
    return FramePreprocessing::CropPreprocessing(
        FramePreprocessing::BinaryPreprocessing(source, cv::COLOR_BGR2GRAY, CV_THRESH_BINARY), crop);
  };
  auto binary_preprocessing_BGR = [](const cv::Mat& source) {
    return FramePreprocessing::BinaryPreprocessing(source, cv::COLOR_BGR2GRAY, CV_THRESH_BINARY);
  };

  auto binary_preprocessing_RGB = [](const cv::Mat& source) {
    return FramePreprocessing::BinaryPreprocessing(source, cv::COLOR_RGB2GRAY, CV_THRESH_BINARY);
  };

  // fill preprocessing list
  switch (optimization_type) {
    case OptimizationType::kZero: {
      preprocessing_lists_.emplace_back(ScreenPreprocessing::kNone, empty);
      break;
    }
    case OptimizationType::kSimple: {
      preprocessing_lists_.emplace_back(ScreenPreprocessing::kCrop, crop_preprocessing);
      preprocessing_lists_.emplace_back(ScreenPreprocessing::kBinarized, binary_preprocessing_BGR);
      preprocessing_lists_.emplace_back(ScreenPreprocessing::kNone, empty);
      break;
    }
    case OptimizationType::kAdvanced: {
      preprocessing_lists_.emplace_back(ScreenPreprocessing::kCrop, crop_preprocessing);
      preprocessing_lists_.emplace_back(ScreenPreprocessing::kBinarized, binary_preprocessing_BGR);
      preprocessing_lists_.emplace_back(ScreenPreprocessing::kNone, empty);
      preprocessing_lists_.emplace_back(ScreenPreprocessing::kBinarizedRGB, binary_preprocessing_RGB);
      break;
    }
  }
}

std::string TextDetectorDecorator::GetCurrentPreprocessingName() const noexcept {
  std::string name_for_logger{"Unknown Preprocessing"};
  switch (active_preprocessing_) {
    case ScreenPreprocessing::kNone: {
      name_for_logger = "without preprocessing";
      break;
    }
    case ScreenPreprocessing::kCrop: {
      name_for_logger = "with cropping frame";
      break;
    }
    case ScreenPreprocessing::kBinarized: {
      name_for_logger = "with binary converting frame";
      break;
    }
    case ScreenPreprocessing::kOTSUBinarized: {
      name_for_logger = "with binary converting frame + OTSU method";
      break;
    }
    case ScreenPreprocessing::kBinarizedRGB: {
      name_for_logger = "with binary converting frame from rgb";
    }
  }
  return name_for_logger;
}

TextDetectorDecorator::OptimizationType TextDetectorDecorator::GetOptimizationTypeByName(const std::string& name) const
    noexcept {
  static const std::map<std::string, OptimizationType> StringsType{{"Zero", OptimizationType::kZero},
                                                                   {"Simple", OptimizationType::kSimple},
                                                                   {"Advanced", OptimizationType::kAdvanced}

  };
  auto it = StringsType.find(name);

  auto type = OptimizationType::kZero;
  if (it == StringsType.end()) {
    logger::warn("[TextDetectorDecorator] Error optimization name {}. Optimization disabled.", name);
  } else {
    type = it->second;
  }

  return type;
}

void TextDetectorDecorator::PrintLogMessage(TextDetectorDecorator::TypeMessage type) {
  std::string message;
  switch (type) {
    case TypeMessage::kBegin: {
      logger::trace("[TextDetectorDecorator] Start find text on frame {}.", GetCurrentPreprocessingName());
      break;
    }
    case TypeMessage::kFound: {
      logger::trace("[TextDetectorDecorator] Found text on frame {}.", GetCurrentPreprocessingName());
      break;
    }
    case TypeMessage::kNotFound: {
      logger::trace("[TextDetectorDecorator] Not found text on frame {}.", GetCurrentPreprocessingName());
      break;
    }
  }
}

}  // namespace detector
