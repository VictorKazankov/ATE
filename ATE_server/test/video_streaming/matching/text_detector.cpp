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

constexpr auto kTessDataPrefix = "/opt/Tesseract/traineddata/";
constexpr float kMinimalConfidence = 80.f;
const double kHausdorffThreshold = 2 * std::sqrt(2);  // x * sqrt(2) for x pixels border difference

struct TestTextObject {
  cv::Rect position;
  std::string text;
};

template <typename TestObjectContainer>
void RecognizeImageBaseTest(const char* image_file, const TestObjectContainer& expected_objects) {
  const TextDetector text_detector{kTessDataPrefix};

  const cv::Mat matrix = cv::imread(image_file, cv::IMREAD_COLOR);

  ASSERT_FALSE(matrix.empty()) << "Image must be non-empty";
  ASSERT_EQ(matrix.dims, 2) << "Image matrix must be 2-dimensional";
  ASSERT_TRUE(CV_8UC1 == matrix.type() || CV_8UC3 == matrix.type())
      << "Image must have 1 or 3 channels with 8 bits per channel";

  const auto recognition_result = text_detector.Recognize(matrix);
  ASSERT_TRUE(recognition_result) << "The result must be non-null";

  const TextDetectorResultRange range = recognition_result->GetRange(tesseract::RIL_WORD);

  const std::vector<TextObject> data{range.begin(), range.end()};
  const auto expected_size = std::distance(std::begin(expected_objects), std::end(expected_objects));
  ASSERT_EQ(data.size(), expected_size) << "Expected and actual size must be the same";
  auto expected_iter = std::begin(expected_objects);
  for (const auto& object : data) {
    EXPECT_EQ(object.text, expected_iter->text) << "Expected and actual texts must be the same";
    const double distance = HausdorffDistance(object.position, expected_iter->position);
    EXPECT_LE(distance, kHausdorffThreshold) << "Rectangles must be approximately equal";
    EXPECT_GE(object.confidence, kMinimalConfidence) << "Confidence must be greater than " << kMinimalConfidence;
    ++expected_iter;
  }
}

TEST(TextDetectorTest, EnvTessDataPrefix) {
  const TextDetector text_detector{kTessDataPrefix};
  const auto get_result = safe_env::Get(kTessdataPrefixEnvVarName);
  EXPECT_TRUE(get_result.second) << std::quoted(kTessdataPrefixEnvVarName) << " environment variable must be defined";
  EXPECT_EQ(get_result.first.back(), '/')
      << std::quoted(kTessdataPrefixEnvVarName) << " environment variable must contain a slash at the end";
}

TEST(TextDetectorTest, DefaultConstructibleIterator) {
  const TextDetectorResultIterator it1, it2;
  constexpr auto error_message = "2 default constructed iterators must be the same";
  EXPECT_EQ(it1, it2) << error_message;
  EXPECT_FALSE(it1 != it2) << error_message;
}

TEST(TextDetectorTest, IteratorStdAlgorithms) {
  auto find_empty_string = [](TextDetectorResultIterator::reference ref) noexcept { return ref.text.empty(); };
  TextDetectorResultIterator find_result =
      std::find_if(TextDetectorResultIterator{}, TextDetectorResultIterator{}, std::move(find_empty_string));
  EXPECT_EQ(find_result, TextDetectorResultIterator{});
}

TEST(TextDetectorTest, NullTessBaseAPI) {
  const auto multi_dim_image_sizes = {1, 1, 1};

  // invalid_images[0] - Empty image
  // invalid_images[1] - image with alpha chanell
  // invalid_images[2] - multi-dimensional matrix
  const auto invalid_images = {
      cv::Mat{}, cv::Mat{1, 1, CV_8UC4},
      cv::Mat{static_cast<int>(multi_dim_image_sizes.size()), multi_dim_image_sizes.begin(), CV_8UC3}};

  const TextDetector text_detector{kTessDataPrefix};

  for (const cv::Mat& image : invalid_images) {
    EXPECT_THROW(text_detector.Recognize(image), std::invalid_argument)
        << "TextDetector::Recognize must throw invalid_argument";
    EXPECT_THROW(TextDetectorResult{image}, std::invalid_argument)
        << "TextDetectorResult constructor must throw invalid_argument";
  }

  bool invalid_argument_occurs = false;
  try {
    TextDetectorResultRange range{nullptr, tesseract::RIL_BLOCK};
  } catch (const std::invalid_argument&) {
    invalid_argument_occurs = true;
  }
  EXPECT_TRUE(invalid_argument_occurs)
      << "TextDetectorResultRange constructor with null tess pointer must throw invalid_argument";

  invalid_argument_occurs = false;
  try {
    TextDetectorResultIterator iterator{nullptr, tesseract::RIL_BLOCK};
  } catch (const std::invalid_argument&) {
    invalid_argument_occurs = true;
  }
  EXPECT_TRUE(invalid_argument_occurs)
      << "TextDetectorResultIterator constructor with null tess pointer must throw invalid_argument";
}

TEST(TextDetectorTest, ZeroImage) {
  const cv::Mat zero_image = cv::Mat::zeros(128, 128, CV_8UC3);
  const TextDetector text_detector{kTessDataPrefix};
  auto result = text_detector.Recognize(zero_image);
  ASSERT_TRUE(result) << "The result must be non-null";
  TextDetectorResultRange range = result->GetRange(tesseract::RIL_SYMBOL);
  const std::ptrdiff_t dist = std::distance(range.begin(), range.end());
  EXPECT_EQ(dist, 0) << "The range must be empty";
}

TEST(TextDetectorTest, RecognizeTextAudioFrequency) {
  const TestTextObject expected_objects[] = {{{cv::Point{39, 19}, cv::Size{120, 24}}, "Frequency"}};
  RecognizeImageBaseTest(VHAT_SERVER_TEST_DATA_PATH "/video_streaming/matching/audio_frequency.png", expected_objects);
}

TEST(TextDetectorTest, RecognizeTextClimateDual) {
  const TestTextObject expected_objects[] = {{{cv::Point{55, 17}, cv::Size{67, 20}}, "DUAL"}};
  RecognizeImageBaseTest(VHAT_SERVER_TEST_DATA_PATH "/video_streaming/matching/climate_dual.png", expected_objects);
}

TEST(TextDetectorTest, RecognizeTextKeyActive_0) {
  const TestTextObject expected_objects[] = {{{cv::Point{30, 15}, cv::Size{13, 19}}, "0"}};
  RecognizeImageBaseTest(VHAT_SERVER_TEST_DATA_PATH "/video_streaming/matching/key_0_active.png", expected_objects);
}

TEST(TextDetectorTest, RecognizeTextKeyActive_1) {
  const TestTextObject expected_objects[] = {{{cv::Point{32, 17}, cv::Size{8, 18}}, "1"}};
  RecognizeImageBaseTest(VHAT_SERVER_TEST_DATA_PATH "/video_streaming/matching/key_1_active.png", expected_objects);
}

TEST(TextDetectorTest, RecognizeTextKeyActive_2) {
  const TestTextObject expected_objects[] = {{{cv::Point{32, 16}, cv::Size{12, 19}}, "2"}};
  RecognizeImageBaseTest(VHAT_SERVER_TEST_DATA_PATH "/video_streaming/matching/key_2_active.png", expected_objects);
}

TEST(TextDetectorTest, RecognizeTextKeyActive_3) {
  const TestTextObject expected_objects[] = {{{cv::Point{31, 16}, cv::Size{13, 19}}, "3"}};
  RecognizeImageBaseTest(VHAT_SERVER_TEST_DATA_PATH "/video_streaming/matching/key_3_active.png", expected_objects);
}

TEST(TextDetectorTest, RecognizeTextKeyActive_4) {
  const TestTextObject expected_objects[] = {{{cv::Point{32, 17}, cv::Size{13, 18}}, "4"}};
  RecognizeImageBaseTest(VHAT_SERVER_TEST_DATA_PATH "/video_streaming/matching/key_4_active.png", expected_objects);
}

TEST(TextDetectorTest, RecognizeTextKeyActive_5) {
  const TestTextObject expected_objects[] = {{{cv::Point{30, 16}, cv::Size{13, 18}}, "5"}};
  RecognizeImageBaseTest(VHAT_SERVER_TEST_DATA_PATH "/video_streaming/matching/key_5_active.png", expected_objects);
}

TEST(TextDetectorTest, RecognizeTextKeyActive_6) {
  const TestTextObject expected_objects[] = {{{cv::Point{31, 15}, cv::Size{13, 19}}, "6"}};
  RecognizeImageBaseTest(VHAT_SERVER_TEST_DATA_PATH "/video_streaming/matching/key_6_active.png", expected_objects);
}

TEST(TextDetectorTest, RecognizeTextKeyActive_7) {
  const TestTextObject expected_objects[] = {{{cv::Point{31, 16}, cv::Size{13, 18}}, "7"}};
  RecognizeImageBaseTest(VHAT_SERVER_TEST_DATA_PATH "/video_streaming/matching/key_7_active.png", expected_objects);
}

TEST(TextDetectorTest, RecognizeTextKeyActive_8) {
  const TestTextObject expected_objects[] = {{{cv::Point{31, 16}, cv::Size{13, 19}}, "8"}};
  RecognizeImageBaseTest(VHAT_SERVER_TEST_DATA_PATH "/video_streaming/matching/key_8_active.png", expected_objects);
}

TEST(TextDetectorTest, RecognizeTextKeyActive_9) {
  const TestTextObject expected_objects[] = {{{cv::Point{31, 16}, cv::Size{13, 19}}, "9"}};
  RecognizeImageBaseTest(VHAT_SERVER_TEST_DATA_PATH "/video_streaming/matching/key_9_active.png", expected_objects);
}

TEST(TextDetectorTest, RecognizeTextKeyActive_M) {
  const TestTextObject expected_objects[] = {{{cv::Point{25, 17}, cv::Size{19, 19}}, "M"}};
  RecognizeImageBaseTest(VHAT_SERVER_TEST_DATA_PATH "/video_streaming/matching/key_m_active.png", expected_objects);
}

TEST(TextDetectorTest, RecognizeTextTimeAMPeriod) {
  const TestTextObject expected_objects[] = {{{cv::Point{14, 18}, cv::Size{38, 19}}, "AM"}};
  RecognizeImageBaseTest(VHAT_SERVER_TEST_DATA_PATH "/video_streaming/matching/time_am_period.png", expected_objects);
}

TEST(TextDetectorTest, RecognizeTextTimePMPeriod) {
  const TestTextObject expected_objects[] = {{{cv::Point{16, 19}, cv::Size{36, 19}}, "PM"}};
  RecognizeImageBaseTest(VHAT_SERVER_TEST_DATA_PATH "/video_streaming/matching/time_pm_period.png", expected_objects);
}

}  // namespace
