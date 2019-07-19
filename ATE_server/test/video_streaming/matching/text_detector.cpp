#include "video_streaming/matching/text_detector.h"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iterator>
#include <stdexcept>

#include <gtest/gtest.h>

#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>

#include "utils/safe_env.h"
#include "video_streaming/matching/hausdorff_distance.h"

namespace {

using namespace detector;

constexpr auto kTessDataPrefix = "/usr/local/share/tessdata/";
constexpr float kMinimalConfidence = 60.f;
const double kHausdorffThreshold = 2 * std::sqrt(2);  // x * sqrt(2) for x pixels border difference

struct TestTextObject {
  cv::Rect position;
  std::string text;
};

class TextDetectorTest : public ::testing::Test {
 protected:
  void SetUp() override;
  void TearDown() override;

  std::unique_ptr<detector::TextDetector> text_detector_;
};

void TextDetectorTest::SetUp() {
  text_detector_ = std::make_unique<detector::TextDetector>(kMinimalConfidence, kTessDataPrefix);
}

void TextDetectorTest::TearDown() { text_detector_.reset(); }

TEST(TextDetectorUtilsTest, EnvTessDataPrefix) {
  TextDetector text_detector(kMinimalConfidence, kTessDataPrefix);
  const auto get_result = safe_env::Get(kTessdataPrefixEnvVarName);
  EXPECT_TRUE(get_result.second) << std::quoted(kTessdataPrefixEnvVarName) << " environment variable must be defined";
  EXPECT_EQ(get_result.first.back(), '/') << std::quoted(kTessdataPrefixEnvVarName)
                                          << " environment variable must contain a slash at the end";
}

TEST(TextDetectorUtilsTest, DefaultConstructibleIterator) {
  const TextDetectorResultIterator it1, it2;
  constexpr auto error_message = "2 default constructed iterators must be the same";
  EXPECT_EQ(it1, it2) << error_message;
  EXPECT_FALSE(it1 != it2) << error_message;
}

TEST(TextDetectorUtilsTest, IteratorStdAlgorithms) {
  auto find_empty_string = [](TextDetectorResultIterator::reference ref) noexcept { return ref.text.empty(); };
  TextDetectorResultIterator find_result =
      std::find_if(TextDetectorResultIterator{}, TextDetectorResultIterator{}, std::move(find_empty_string));
  EXPECT_EQ(find_result, TextDetectorResultIterator{});
}

TEST_F(TextDetectorTest, Detect_NoTextToDetect_Exception) {
  const cv::Mat frame =
      cv::imread(VHAT_SERVER_TEST_DATA_PATH "/video_streaming/matching/key_1_active.png", cv::IMREAD_COLOR);
  EXPECT_THROW(text_detector_->Detect(frame, ""), std::invalid_argument);
}

TEST_F(TextDetectorTest, Detect_BadImages_Exception) {
  const auto multi_dim_image_sizes = {1, 1, 1};

  // invalid_images[0] - Empty image
  // invalid_images[1] - image with alpha chanell
  // invalid_images[2] - multi-dimensional matrix
  const auto invalid_images = {
      cv::Mat{}, cv::Mat{1, 1, CV_8UC4},
      cv::Mat{static_cast<int>(multi_dim_image_sizes.size()), multi_dim_image_sizes.begin(), CV_8UC3}};

  for (const cv::Mat& image : invalid_images) {
    EXPECT_THROW(text_detector_->Detect(image, "some text"), std::invalid_argument)
        << "TextDetector::Recognize must throw invalid_argument";
  }
}

void RecognizeImageBaseTest(std::unique_ptr<detector::TextDetector> text_detector, const char* image_file,
                            const TestTextObject& expected) {
  const cv::Mat frame = cv::imread(image_file, cv::IMREAD_COLOR);

  cv::Rect result_rectangle = text_detector->Detect(frame, expected.text);
  const double distance = HausdorffDistance(result_rectangle, expected.position);
  EXPECT_LE(distance, kHausdorffThreshold) << "Rectangles must be approximately equal";
}

TEST_F(TextDetectorTest, Detect_TextAudioFrequency_Success) {
  const TestTextObject expected_object = {{cv::Point{39, 19}, cv::Size{120, 24}}, "Frequency"};
  RecognizeImageBaseTest(std::move(text_detector_),
                         VHAT_SERVER_TEST_DATA_PATH "/video_streaming/matching/audio_frequency.png", expected_object);
}

TEST_F(TextDetectorTest, Detect_TextClimateDual_Success) {
  const TestTextObject expected_object = {{cv::Point{55, 17}, cv::Size{67, 20}}, "DUAL"};
  RecognizeImageBaseTest(std::move(text_detector_),
                         VHAT_SERVER_TEST_DATA_PATH "/video_streaming/matching/climate_dual.png", expected_object);
}

TEST_F(TextDetectorTest, Detect_TextKeyActiveZero_Success) {
  const TestTextObject expected_object = {{cv::Point{30, 15}, cv::Size{13, 19}}, "0"};
  RecognizeImageBaseTest(std::move(text_detector_),
                         VHAT_SERVER_TEST_DATA_PATH "/video_streaming/matching/key_0_active.png", expected_object);
}

TEST_F(TextDetectorTest, Detect_TextKeyActiveOne_Success) {
  const TestTextObject expected_object = {{cv::Point{32, 17}, cv::Size{8, 18}}, "1"};
  RecognizeImageBaseTest(std::move(text_detector_),
                         VHAT_SERVER_TEST_DATA_PATH "/video_streaming/matching/key_1_active.png", expected_object);
}

TEST_F(TextDetectorTest, Detect_TextKeyActiveM_Success) {
  const TestTextObject expected_object = {{cv::Point{25, 17}, cv::Size{19, 19}}, "M"};
  RecognizeImageBaseTest(std::move(text_detector_),
                         VHAT_SERVER_TEST_DATA_PATH "/video_streaming/matching/key_m_active.png", expected_object);
}

TEST_F(TextDetectorTest, Detect_TextTimeAMPeriod_Success) {
  const TestTextObject expected_object = {{cv::Point{14, 18}, cv::Size{38, 19}}, "AM"};
  RecognizeImageBaseTest(std::move(text_detector_),
                         VHAT_SERVER_TEST_DATA_PATH "/video_streaming/matching/time_am_period.png", expected_object);
}

TEST_F(TextDetectorTest, Detect_TextTimePMPeriod_Success) {
  const TestTextObject expected_object = {{cv::Point{16, 19}, cv::Size{36, 19}}, "PM"};
  RecognizeImageBaseTest(std::move(text_detector_),
                         VHAT_SERVER_TEST_DATA_PATH "/video_streaming/matching/time_pm_period.png", expected_object);
}

TEST_F(TextDetectorTest, Detect_PhraseMobileApp_Success) {
  const TestTextObject expected_object = {cv::Rect{722, 159, 236, 42}, "Mobile App"};
  RecognizeImageBaseTest(std::move(text_detector_),
                         VHAT_SERVER_TEST_DATA_PATH "/video_streaming/matching/few_words.png", expected_object);
}

TEST_F(TextDetectorTest, Detect_PhraseFindMobiWithMissedEndOfTheLastWord_Success) {
  const TestTextObject expected_object = {cv::Rect{395, 159, 222, 35}, "Find Mobi"};
  RecognizeImageBaseTest(std::move(text_detector_),
                         VHAT_SERVER_TEST_DATA_PATH "/video_streaming/matching/few_words.png", expected_object);
}

TEST_F(TextDetectorTest, Detect_WordsOnFrame_Success) {
  using textOnScreenshot = std::pair<std::string, std::string>;
  auto list_text_screen = {textOnScreenshot{"Mobile App", "few_words.png"},
                           textOnScreenshot{"Find Mobi", "few_words.png"},
                           textOnScreenshot{"Sources", "audio_screen.png"}};

  for (const auto it : list_text_screen) {
    std::string text = it.first, screenshot_name = it.second;

    const auto error_message = text + " not found on " + screenshot_name;
    const cv::Mat frame =
        cv::imread(VHAT_SERVER_TEST_DATA_PATH "/video_streaming/matching/" + screenshot_name, cv::IMREAD_COLOR);
    EXPECT_FALSE(text_detector_->Detect(frame, text) == cv::Rect()) << error_message;
  }
}

}  // namespace
