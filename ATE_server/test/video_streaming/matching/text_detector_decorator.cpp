#include <memory>
#include <string>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

// Helper macros for testing private section
#define protected public
#define private public
#include "video_streaming/matching/text_detector_decorator.h"

namespace {

using ::testing::_;  // Matcher for parameters
using ::testing::Return;

// constants
const std::string kSync3{"sync3"};
const std::string kSync4{"sync4"};
const std::string kOptimizationTypeZero{"Zero"};
const std::string kOptimizationTypeSimple{"Simple"};
const std::string kOptimizationTypeAdvanced{"Advanced"};
const std::string kFramePath = VHAT_SERVER_TEST_DATA_PATH "/video_streaming/matching/hmi_screenshot.png";

/*--------------------------- FramePreprocessing tests---------------------------*/
TEST(FramePreprocessing, Preprocessing_Crop_Success) {
  auto frame = cv::imread(kFramePath);
  auto crop_coefficient = 1.5;

  // Etalon crop
  auto crop_etalon = cv::Mat{};
  auto crop_colls = static_cast<int>(frame.cols / crop_coefficient);
  auto crop_rows = static_cast<int>(frame.rows / crop_coefficient);
  cv::resize(frame, crop_etalon, cv::Size(crop_colls, crop_rows));

  // decorator crop
  auto crop_test = detector::FramePreprocessing::CropPreprocessing(frame, crop_coefficient);

  EXPECT_TRUE(sum(crop_etalon != crop_test) == cv::Scalar(0, 0, 0, 0));
}

TEST(FramePreprocessing, Preprocessing_Binary_Success) {
  auto frame = cv::imread(kFramePath);

  // Etalon binarized
  cv::Mat binarized_etalon{};
  cv::Mat gray_screen{};
  cv::cvtColor(frame, gray_screen, cv::COLOR_BGR2GRAY);
  // convert to binary format
  cv::threshold(gray_screen, binarized_etalon, 127, 255, CV_THRESH_BINARY);

  // decorator binarized
  auto bin_test = detector::FramePreprocessing::BinaryPreprocessing(frame, cv::COLOR_BGR2GRAY, CV_THRESH_BINARY);

  EXPECT_TRUE(cv::countNonZero((binarized_etalon != bin_test) == 0));
}

TEST(FramePreprocessing, Preprocessing_BinaryOTSU_Success) {
  auto frame = cv::imread(kFramePath);

  // Etalon binarizedOTSU
  cv::Mat binarized_etalon{};
  cv::Mat gray_screen{};
  cv::cvtColor(frame, gray_screen, cv::COLOR_BGR2GRAY);
  // convert to binary format
  cv::threshold(gray_screen, binarized_etalon, 127, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);

  // decorator binarized
  auto bin_test =
      detector::FramePreprocessing::BinaryPreprocessing(frame, cv::COLOR_BGR2GRAY, CV_THRESH_BINARY | CV_THRESH_OTSU);

  EXPECT_TRUE(cv::countNonZero((binarized_etalon != bin_test) == 0));
}

/*---------------------------TextDetectorDecorator tests---------------------------*/

// cv::Math comparator
MATCHER_P(OpenCVMatMatches, expected, "Match arg cv::Mat to be equal to expected") {
  if (arg.size() != expected.size()) {
    return false;
  }
  auto diff = (arg != expected);
  return cv::countNonZero(diff) == 0;
}

/**
 * @brief Mock class
 */
class MockTextDetector : public detector::Detector<std::string> {
 public:
  MOCK_METHOD2(Detect, cv::Rect(const cv::Mat&, const std::string&));
};

TEST(TextDetectorDecoratorTest, Construct_Success) {
  ASSERT_NO_THROW(detector::TextDetectorDecorator text_detector(std::make_unique<MockTextDetector>(), kSync4,
                                                                kOptimizationTypeAdvanced));
  ASSERT_NO_THROW(detector::TextDetectorDecorator text_detector(std::make_unique<MockTextDetector>(), kSync4,
                                                                kOptimizationTypeAdvanced));
}

TEST(TextDetectorDecoratorTest, Construct_Exception) {
  ASSERT_THROW(detector::TextDetectorDecorator text_detector(nullptr, kSync4, kOptimizationTypeAdvanced),
               std::runtime_error);
  ASSERT_THROW(detector::TextDetectorDecorator text_detector(std::make_unique<MockTextDetector>(), "",
                                                             kOptimizationTypeAdvanced),
               std::runtime_error);
}

TEST(TextDetectorDecoratorTest, OptimizationType_Success) {
  detector::TextDetectorDecorator text_detector(std::make_unique<MockTextDetector>(), kSync4,
                                                kOptimizationTypeAdvanced);

  EXPECT_EQ(detector::TextDetectorDecorator::OptimizationType::kAdvanced,
            text_detector.GetOptimizationTypeByName(kOptimizationTypeAdvanced))
      << "Incorrect getting optimization type.";
}

TEST(TextDetectorDecoratorTest, GetCurrentPreprocessingName_Success) {
  detector::TextDetectorDecorator text_detector(std::make_unique<MockTextDetector>(), kSync4,
                                                kOptimizationTypeZero);
  EXPECT_EQ("without preprocessing", text_detector.GetCurrentPreprocessingName());

  text_detector.active_preprocessing_ = detector::TextDetectorDecorator::ScreenPreprocessing::kOTSUBinarized;
  EXPECT_EQ("with binary converting frame + OTSU method", text_detector.GetCurrentPreprocessingName());

  text_detector.active_preprocessing_ = detector::TextDetectorDecorator::ScreenPreprocessing::kBinarizedRGB;
  EXPECT_EQ("with binary converting frame from rgb", text_detector.GetCurrentPreprocessingName());
}

TEST(TextDetectorDecoratorTest, Sync3FillPreprocessingList_Success) {
  using sp = detector::TextDetectorDecorator::ScreenPreprocessing;
  std::vector<sp> etalon_none{sp::kNone};
  std::vector<sp> etalon_simple_advanced{sp::kNone, sp::kOTSUBinarized};
  std::vector<sp> test_optimization_list;
  // fill test list
  auto fill_lists = [&test_optimization_list](
                        std::vector<std::pair<detector::TextDetectorDecorator::ScreenPreprocessing,
                                              detector::TextDetectorDecorator::PreprocessingFunctionType>>
                            detector_list) {
    test_optimization_list.clear();
    for (const auto& it : detector_list) {
      test_optimization_list.push_back(it.first);
    }
  };

  detector::TextDetectorDecorator text_detector_none(std::make_unique<MockTextDetector>(), kSync3,
                                                     kOptimizationTypeZero);
  fill_lists(text_detector_none.preprocessing_lists_);
  EXPECT_EQ(etalon_none, test_optimization_list)
      << "Incorrect fill preprocessing list for sync3 for zero optimization level.";

  detector::TextDetectorDecorator text_detector_simple(std::make_unique<MockTextDetector>(), kSync3,
                                                       kOptimizationTypeSimple);
  fill_lists(text_detector_simple.preprocessing_lists_);
  EXPECT_EQ(etalon_simple_advanced, test_optimization_list)
      << "Incorrect fill preprocessing list for sync3 for simple optimization level.";
  ;

  detector::TextDetectorDecorator text_detector_advanced(std::make_unique<MockTextDetector>(), kSync3,
                                                         kOptimizationTypeAdvanced);
  fill_lists(text_detector_advanced.preprocessing_lists_);
  EXPECT_EQ(etalon_simple_advanced, test_optimization_list)
      << "Incorrect fill preprocessing list for sync3 for advanced optimization level.";
}

TEST(TextDetectorDecoratorTest, Sync4FillPreprocessingList_Success) {
  using sp = detector::TextDetectorDecorator::ScreenPreprocessing;
  std::vector<sp> etalon_none{sp::kNone};
  std::vector<sp> etalon_simple{sp::kCrop, sp::kBinarized, sp::kNone};
  std::vector<sp> etalon_advanced{sp::kCrop, sp::kBinarized, sp::kNone, sp::kBinarizedRGB};
  std::vector<sp> test_optimization_list;
  // fill test list
  auto fill_lists = [&test_optimization_list](
                        std::vector<std::pair<detector::TextDetectorDecorator::ScreenPreprocessing,
                                              detector::TextDetectorDecorator::PreprocessingFunctionType>>
                            detector_list) {
    test_optimization_list.clear();
    for (const auto& it : detector_list) {
      test_optimization_list.push_back(it.first);
    }
  };

  detector::TextDetectorDecorator text_detector_none(std::make_unique<MockTextDetector>(), kSync4,
                                                     kOptimizationTypeZero);
  fill_lists(text_detector_none.preprocessing_lists_);
  EXPECT_EQ(etalon_none, test_optimization_list)
      << "Incorrect fill preprocessing list for sync4 for zero optimization level.";

  detector::TextDetectorDecorator text_detector_simple(std::make_unique<MockTextDetector>(), kSync4,
                                                       kOptimizationTypeSimple);
  fill_lists(text_detector_simple.preprocessing_lists_);
  EXPECT_EQ(etalon_simple, test_optimization_list)
      << "Incorrect fill preprocessing list for sync4 for simple optimization level.";

  detector::TextDetectorDecorator text_detector_advanced(std::make_unique<MockTextDetector>(), kSync4,
                                                         kOptimizationTypeAdvanced);
  fill_lists(text_detector_advanced.preprocessing_lists_);
  EXPECT_EQ(etalon_advanced, test_optimization_list)
      << "Incorrect fill preprocessing list for sync4 for advanced optimization level.";
}

TEST(TextDetectorDecoratorTest, CorrectingArea_Succes) {
  const cv::Rect etalon_area_crop{150, 150, 840, 480};
  const cv::Rect etalon_area_none{100, 100, 840, 480};
  cv::Rect test_area_none{100, 100, 840, 480};
  cv::Rect test_area_crop{100, 100, 840, 480};

  detector::TextDetectorDecorator text_detector(std::make_unique<MockTextDetector>(), kSync4, kOptimizationTypeZero);
  ASSERT_FLOAT_EQ(1.5, text_detector.crop_coefficient_) << "Incorrect crop data.";

  text_detector.active_preprocessing_ = detector::TextDetectorDecorator::ScreenPreprocessing::kCrop;
  text_detector.CorrectionArea(test_area_crop);
  EXPECT_TRUE(etalon_area_crop == test_area_crop) << "Detect area is and etalon area in not equal.";

  text_detector.active_preprocessing_ = detector::TextDetectorDecorator::ScreenPreprocessing::kNone;
  text_detector.CorrectionArea(test_area_none);
  EXPECT_TRUE(etalon_area_none == test_area_none) << "Detect area is and etalon area in not equal.";
}

TEST(TextDetectorDecoratorTest, Detect_EmtyPattern_Exception) {
  detector::TextDetectorDecorator decorator(std::make_unique<MockTextDetector>(), kSync4, kOptimizationTypeZero);
  EXPECT_EQ(cv::Rect{}, decorator.Detect(cv::Mat{}, "Hello")) << "Detect area must be empty.";
  EXPECT_EQ(cv::Rect{}, decorator.Detect(cv::Mat{}, "")) << "Detect area must be empty.";
}

TEST(TextDetectorDecoratorTest, No_Detect_Success) {
  auto mock = std::make_unique<MockTextDetector>();
  EXPECT_CALL(*mock, Detect(OpenCVMatMatches(cv::Mat{}), "Hello")).Times(0);

  detector::TextDetectorDecorator decorator(std::move(mock), kSync4, kOptimizationTypeZero);
  decorator.preprocessing_lists_.clear();
  auto detect_area = decorator.Detect(cv::Mat{}, "Hello");
  EXPECT_TRUE(detect_area.empty()) << "Detect area must be empty.";
}

TEST(TextDetectorDecoratorTest, Detect_Grayscale_No_Throw_Success) {
  auto frame = cv::imread(kFramePath, cv::IMREAD_GRAYSCALE);
  auto mock = std::make_unique<MockTextDetector>();
  const cv::Rect etalon_area{150, 150, 840, 480};
  EXPECT_CALL(*mock, Detect(_, _)).Times(1).WillOnce(Return(etalon_area));

  detector::TextDetectorDecorator decorator(std::move(mock), kSync4, kOptimizationTypeZero);
  EXPECT_NO_THROW(decorator.Detect(frame, "Hello"));
}

TEST(TextDetectorDecoratorTest, Detect_No_Optimization_Success) {
  auto frame = cv::imread(kFramePath);
  auto mock = std::make_unique<MockTextDetector>();
  const cv::Rect etalon_area{150, 150, 840, 480};
  EXPECT_CALL(*mock, Detect(_, _)).Times(1).WillOnce(Return(etalon_area));

  detector::TextDetectorDecorator decorator(std::move(mock), kSync4, kOptimizationTypeZero);
  auto detect_area = decorator.Detect(frame, "Hello");
  EXPECT_TRUE(etalon_area == detect_area) << "Detect area is and etalon area in not equal.";
  EXPECT_FALSE(detect_area.empty()) << "Detect area must be empty.";
}

TEST(TextDetectorDecoratorTest, Detect_Optimization_Success) {
  auto frame = cv::imread(kFramePath);
  auto mock = std::make_unique<MockTextDetector>();
  EXPECT_CALL(*mock, Detect(_, _)).Times(3).WillRepeatedly(Return(cv::Rect{}));

  detector::TextDetectorDecorator decorator(std::move(mock), kSync4, kOptimizationTypeSimple);
  auto detect_area = decorator.Detect(frame, "Hello");
  EXPECT_TRUE(detect_area.empty()) << "Detect area must be empty.";
}

}  // namespace
