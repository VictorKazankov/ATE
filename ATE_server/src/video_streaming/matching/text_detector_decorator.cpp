#include "video_streaming/matching/text_detector_decorator.h"

#include <map>

#include <opencv2/imgproc.hpp>

#include "logger/logger.h"

namespace detector {

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
                                             const std::string& preprocessing_lists)
    : text_detector_(std::move(text_detector)) {
  // runtime error if detector is nullptr
  if (text_detector_ == nullptr) {
    logger::critical("[TextDetectorDecorator] Text detector not initialized!");
    throw std::runtime_error("Text detector is nullptr");
  }

  // fill preprocessing steps
  FillPreprocessingList(GetPreprocessingList(preprocessing_lists));
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

  // find pattern with preprocessing on color frame
  if (frame.channels() > 1) {
    // without any preprocessing
    if (preprocessing_lists_.empty()) {
      text_detector_->Detect(frame, pattern);
    }
    // with preprocessing
    else {
      for (const auto& preprocessing_iter : preprocessing_lists_) {
        if (preprocessing_iter.first == ScreenPreprocessing::kNone || preprocessing_iter.second == nullptr) {
          logger::trace("[TextDetectorDecorator] Start find text on frame {}.",
                        GetCurrentPreprocessingDescription(preprocessing_iter.first));
          detected_area = text_detector_->Detect(frame, pattern);
        } else {
          // set optimization
          logger::trace("[TextDetectorDecorator] Start find text on frame {}.",
                        GetCurrentPreprocessingDescription(preprocessing_iter.first));
          detected_area = text_detector_->Detect(preprocessing_iter.second(frame), pattern);
        }
        // if find object - break out from loop
        if (!detected_area.empty()) {
          CorrectionArea(detected_area, preprocessing_iter.first);
          logger::trace("[TextDetectorDecorator] Found text on frame {}.",
                        GetCurrentPreprocessingDescription(preprocessing_iter.first));
          break;
        }
        logger::trace("[TextDetectorDecorator] Not found text on frame {}.",
                      GetCurrentPreprocessingDescription(preprocessing_iter.first));
      }
    }
  }
  // if grayscale mode - find without preprocessing
  else {
    logger::trace("[TextDetectorDecorator] Find without preprocessing. Frame is grayscale.");
    detected_area = text_detector_->Detect(frame, pattern);
  }

  return detected_area;
}

void TextDetectorDecorator::CorrectionArea(cv::Rect& area, ScreenPreprocessing active_preprocessing) const noexcept {
  if (active_preprocessing == ScreenPreprocessing::kBinarizedCrop) {
    area.x = static_cast<int>(area.x * crop_coefficient_);
    area.y = static_cast<int>(area.y * crop_coefficient_);
  }
}

void TextDetectorDecorator::FillPreprocessingList(const std::vector<ScreenPreprocessing>& list_of_preprocessing_type) {
  if (list_of_preprocessing_type.empty()) {
    preprocessing_lists_.emplace_back(ScreenPreprocessing::kNone, [](const cv::Mat& source) { return source; });
    return;
  }
  // fill lists
  for (const auto& it : list_of_preprocessing_type) {
    switch (it) {
      case ScreenPreprocessing::kNone: {
        preprocessing_lists_.emplace_back(ScreenPreprocessing::kNone, [](const cv::Mat& source) { return source; });
        break;
      }
      case ScreenPreprocessing::kBinarizedCrop: {
        preprocessing_lists_.emplace_back(
            ScreenPreprocessing::kBinarizedCrop, [crop = this->crop_coefficient_](const cv::Mat& source) {
              return FramePreprocessing::CropPreprocessing(
                  FramePreprocessing::BinaryPreprocessing(source, cv::COLOR_BGR2GRAY, CV_THRESH_BINARY), crop);
            });
        break;
      }
      case ScreenPreprocessing::kBinarized: {
        preprocessing_lists_.emplace_back(ScreenPreprocessing::kBinarized, [](const cv::Mat& source) {
          return FramePreprocessing::BinaryPreprocessing(source, cv::COLOR_BGR2GRAY, CV_THRESH_BINARY);
        });
        break;
      }
      case ScreenPreprocessing::kBinarizedOTSU: {
        preprocessing_lists_.emplace_back(ScreenPreprocessing::kBinarizedOTSU, [](const cv::Mat& source) {
          return FramePreprocessing::BinaryPreprocessing(source, cv::COLOR_BGR2GRAY, CV_THRESH_BINARY | CV_THRESH_OTSU);
        });
        break;
      }
      case ScreenPreprocessing::kBinarizedRGB: {
        preprocessing_lists_.emplace_back(ScreenPreprocessing::kBinarizedRGB, [](const cv::Mat& source) {
          return FramePreprocessing::BinaryPreprocessing(source, cv::COLOR_RGB2GRAY, CV_THRESH_BINARY);
        });
        break;
      }
      case ScreenPreprocessing::kUnknown: {
        break;
      }
    }
  }
}

std::string TextDetectorDecorator::GetCurrentPreprocessingDescription(ScreenPreprocessing active_preprocessing) const
    noexcept {
  std::string name_for_logger{"Unknown Preprocessing"};
  switch (active_preprocessing) {
    case ScreenPreprocessing::kNone: {
      name_for_logger = "without preprocessing";
      break;
    }
    case ScreenPreprocessing::kBinarizedCrop: {
      name_for_logger = "with cropping frame";
      break;
    }
    case ScreenPreprocessing::kBinarized: {
      name_for_logger = "with binary converting frame";
      break;
    }
    case ScreenPreprocessing::kBinarizedOTSU: {
      name_for_logger = "with binary converting frame + OTSU method";
      break;
    }
    case ScreenPreprocessing::kBinarizedRGB: {
      name_for_logger = "with binary converting frame from rgb";
      break;
    }
    case ScreenPreprocessing::kUnknown: {
      break;
    }
  }

  return name_for_logger;
}

TextDetectorDecorator::ScreenPreprocessing TextDetectorDecorator::GetScreenPreprocessingByName(
    const std::string& name) const noexcept {
  static const std::map<std::string, ScreenPreprocessing> StringsType{
      {"None", ScreenPreprocessing::kNone},
      {"BinarizedCrop", ScreenPreprocessing::kBinarizedCrop},
      {"Binarized", ScreenPreprocessing::kBinarized},
      {"BinarizedOTSU", ScreenPreprocessing::kBinarizedOTSU},
      {"BinarizedRGB", ScreenPreprocessing::kBinarizedRGB},
  };

  auto it = StringsType.find(name);

  auto type = ScreenPreprocessing::kUnknown;
  if (it == StringsType.end()) {
    logger::warn("[TextDetectorDecorator] Unknown screen reprocessing procedure: {}", name);
  } else {
    type = it->second;
  }

  return type;
}

std::vector<TextDetectorDecorator::ScreenPreprocessing> TextDetectorDecorator::GetPreprocessingList(
    const std::string& list_of_preprocessing) const noexcept {
  std::vector<std::string> split_list{};

  // Split function
  [list = &split_list](std::string string_for_split, const std::string& delimiter) {
    if (string_for_split.empty()) {
      return;
    }

    // parsing
    size_t pos = 0;
    while ((pos = string_for_split.find(delimiter)) != std::string::npos) {
      list->emplace_back(string_for_split.substr(0, pos));
      string_for_split.erase(0, pos + delimiter.length());
    }
    list->emplace_back(string_for_split);
  }(list_of_preprocessing, "+");

  // fill preprocessing list
  std::vector<ScreenPreprocessing> preprocessing_list{};
  for (const auto& it : split_list) {
    preprocessing_list.emplace_back(GetScreenPreprocessingByName(it));
  }

  return preprocessing_list;
}

}  // namespace detector
