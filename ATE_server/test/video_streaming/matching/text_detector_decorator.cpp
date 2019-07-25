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
  ASSERT_NO_THROW(detector::TextDetectorDecorator text_detector(std::make_unique<MockTextDetector>(), "None"));
  ASSERT_NO_THROW(detector::TextDetectorDecorator text_detector(std::make_unique<MockTextDetector>(),
                                                                "None+BinarizedOTSU+BinarizedRGB"));
}

TEST(TextDetectorDecoratorTest, Construct_Exception) {
  ASSERT_THROW(detector::TextDetectorDecorator text_detector(nullptr, "None"), std::runtime_error);
}

TEST(TextDetectorDecoratorTest, GetCurrentPreprocessingName_Success) {
  detector::TextDetectorDecorator text_detector(std::make_unique<MockTextDetector>(),
                                                "None+BinarizedOTSU+BinarizedRGB");
  EXPECT_EQ("Unknown Preprocessing", text_detector.GetCurrentPreprocessingDescription(
                                         detector::TextDetectorDecorator::ScreenPreprocessing::kUnknown));

  EXPECT_EQ("with binary converting frame + OTSU method",
            text_detector.GetCurrentPreprocessingDescription(
                detector::TextDetectorDecorator::ScreenPreprocessing::kBinarizedOTSU));

  EXPECT_EQ("with binary converting frame from rgb",
            text_detector.GetCurrentPreprocessingDescription(
                detector::TextDetectorDecorator::ScreenPreprocessing::kBinarizedRGB));
}

TEST(TextDetectorDecoratorTest, FillPreprocessingList_Success) {
  using sp = detector::TextDetectorDecorator::ScreenPreprocessing;
  std::vector<sp> etalon_none{sp::kNone};
  std::vector<sp> etalon_simple{sp::kBinarizedCrop, sp::kBinarized, sp::kNone};
  std::vector<sp> etalon_advanced{sp::kBinarizedCrop, sp::kBinarized, sp::kNone, sp::kBinarizedRGB};
  std::vector<sp> test_optimization_list;
  // fill test list
  auto fill_lists =
      [&test_optimization_list](
          const std::vector<std::pair<detector::TextDetectorDecorator::ScreenPreprocessing,
                                      detector::TextDetectorDecorator::PreprocessingFunctionType>>& detector_list) {
        test_optimization_list.clear();
        for (const auto& it : detector_list) {
          test_optimization_list.push_back(it.first);
        }
      };

  detector::TextDetectorDecorator text_detector(std::make_unique<MockTextDetector>(), "");
  fill_lists(text_detector.preprocessing_lists_);
  EXPECT_EQ(etalon_none, test_optimization_list) << "Incorrect fill preprocessing list for empty preprocessing string";

  text_detector.preprocessing_lists_.clear();
  text_detector.FillPreprocessingList(text_detector.GetPreprocessingList("None"));
  fill_lists(text_detector.preprocessing_lists_);
  EXPECT_EQ(etalon_none, test_optimization_list) << "Incorrect fill preprocessing list for 'None' preprocessing string";

  text_detector.preprocessing_lists_.clear();
  text_detector.FillPreprocessingList(text_detector.GetPreprocessingList("BinarizedCrop+Binarized+None"));
  fill_lists(text_detector.preprocessing_lists_);
  EXPECT_EQ(etalon_simple, test_optimization_list)
      << "Incorrect fill preprocessing list for 'BinarizedCrop+Binarized+None' preprocessing string";


  text_detector.preprocessing_lists_.clear();
  text_detector.FillPreprocessingList(text_detector.GetPreprocessingList("BinarizedCrop+Binarized+None+BinarizedRGB"));
  fill_lists(text_detector.preprocessing_lists_);
  EXPECT_EQ(etalon_advanced, test_optimization_list)
      << "Incorrect fill preprocessing list for 'BinarizedCrop+Binarized+None+BinarizedRGB' preprocessing string";


  std::vector<sp> etalon_bad_init{sp::kBinarizedCrop, sp::kNone};
  text_detector.preprocessing_lists_.clear();
  text_detector.FillPreprocessingList(text_detector.GetPreprocessingList("BinarizedCrop+BadBad+None+BinarizedRGBBad"));
  fill_lists(text_detector.preprocessing_lists_);
  EXPECT_EQ(etalon_bad_init, test_optimization_list)
            << "Incorrect fill preprocessing list for 'BinarizedCrop+None' preprocessing string";

}

TEST(TextDetectorDecoratorTest, CorrectingArea_Succes) {
  const cv::Rect etalon_area_crop{150, 150, 840, 480};
  const cv::Rect etalon_area_none{100, 100, 840, 480};
  cv::Rect test_area_none{100, 100, 840, 480};
  cv::Rect test_area_crop{100, 100, 840, 480};

  detector::TextDetectorDecorator text_detector(std::make_unique<MockTextDetector>(), "None");
  ASSERT_FLOAT_EQ(1.5, text_detector.crop_coefficient_) << "Incorrect crop data.";

  text_detector.CorrectionArea(test_area_crop, detector::TextDetectorDecorator::ScreenPreprocessing::kBinarizedCrop);
  EXPECT_TRUE(etalon_area_crop == test_area_crop) << "Detect area is and etalon area in not equal.";

  text_detector.CorrectionArea(test_area_none, detector::TextDetectorDecorator::ScreenPreprocessing::kNone);
  EXPECT_TRUE(etalon_area_none == test_area_none) << "Detect area is and etalon area in not equal.";
}

TEST(TextDetectorDecoratorTest, Detect_Emty_Pattern_Exception) {
  detector::TextDetectorDecorator decorator(std::make_unique<MockTextDetector>(), "None");
  EXPECT_EQ(cv::Rect{}, decorator.Detect(cv::Mat{}, "Hello")) << "Detect area must be empty.";
  EXPECT_EQ(cv::Rect{}, decorator.Detect(cv::Mat{}, "")) << "Detect area must be empty.";
}

TEST(TextDetectorDecoratorTest, Pattern_No_Detected_Success) {
  auto mock = std::make_unique<MockTextDetector>();
  EXPECT_CALL(*mock, Detect(OpenCVMatMatches(cv::Mat{}), "Hello")).Times(0);

  detector::TextDetectorDecorator decorator(std::move(mock), "None");
  decorator.preprocessing_lists_.clear();
  auto detect_area = decorator.Detect(cv::Mat{}, "Hello");
  EXPECT_TRUE(detect_area.empty()) << "Detect area must be empty.";
}

TEST(TextDetectorDecoratorTest, Patern_Detected_Grayscale_No_Throw_Success) {
  auto frame = cv::imread(kFramePath, cv::IMREAD_GRAYSCALE);
  auto mock = std::make_unique<MockTextDetector>();
  const cv::Rect etalon_area{150, 150, 840, 480};
  EXPECT_CALL(*mock, Detect(_, _)).Times(1).WillOnce(Return(etalon_area));

  detector::TextDetectorDecorator decorator(std::move(mock), "None");
  EXPECT_NO_THROW(decorator.Detect(frame, "Hello"));
}

TEST(TextDetectorDecoratorTest, Pattern_Detected_No_Preprocessing_Success) {
  auto frame = cv::imread(kFramePath);
  auto mock = std::make_unique<MockTextDetector>();
  const cv::Rect etalon_area{150, 150, 840, 480};
  EXPECT_CALL(*mock, Detect(_, _)).Times(1).WillOnce(Return(etalon_area));

  detector::TextDetectorDecorator decorator(std::move(mock), "None");
  auto detect_area = decorator.Detect(frame, "Hello");
  EXPECT_TRUE(etalon_area == detect_area) << "Detect area is and etalon area in not equal.";
  EXPECT_FALSE(detect_area.empty()) << "Detect area must be empty.";
}

TEST(TextDetectorDecoratorTest, No_Pattern_Detected_Preprocessing_Success) {
  auto frame = cv::imread(kFramePath);
  auto mock = std::make_unique<MockTextDetector>();
  EXPECT_CALL(*mock, Detect(_, _)).Times(3).WillRepeatedly(Return(cv::Rect{}));

  detector::TextDetectorDecorator decorator(std::move(mock), "BinarizedCrop+Binarized+None");
  auto detect_area = decorator.Detect(frame, "Hello");
  EXPECT_TRUE(detect_area.empty()) << "Detected area must be empty.";
}

TEST(TextDetectorDecoratorTest, Pattern_Detected_Preprocessing_Success) {
  auto frame = cv::imread(kFramePath);
  const cv::Rect etalon_area{150, 150, 840, 480};
  auto mock = std::make_unique<MockTextDetector>();
  EXPECT_CALL(*mock, Detect(_, _)).Times(2).WillOnce(Return(cv::Rect{})).WillOnce(Return(etalon_area));

  detector::TextDetectorDecorator decorator(std::move(mock), "BinarizedCrop+Binarized+None");
  auto detect_area = decorator.Detect(frame, "Hello");
  EXPECT_FALSE(detect_area.empty()) << "Detect area must be non empty.";
}

}  // namespace
