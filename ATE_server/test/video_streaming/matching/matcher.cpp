#include "video_streaming/matching/matcher.h"

#include <memory>
#include <string>
#include <utility>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/imgcodecs.hpp>

#include <recognition/detector.h>

#include "ate_error.h"
#include "utils/video_status.h"
#include "video_streaming/streamer.h"

namespace {

using ::testing::_;
using ::testing::Invoke;
using ::testing::Return;
using ::testing::Test;

using namespace detector;
using namespace utils;
template <typename T>
class MockDetector : public Detector<T> {
 public:
  ~MockDetector() override = default;

  MOCK_METHOD3_T(Detect, cv::Rect(const cv::Mat& frame, const T& pattern, bool match_color));
  MOCK_METHOD3_T(DetectAll, std::vector<cv::Rect>(const cv::Mat& frame, const T& pattern, bool match_color));
};

template <>
class MockDetector<std::string> : public Detector<std::string> {
 public:
  ~MockDetector() override = default;

  MOCK_METHOD2_T(Detect, cv::Rect(const cv::Mat& frame, const std::string& pattern));
  MOCK_METHOD1_T(ExtractText, std::string(const cv::Mat& frame));
};

class MockStreamer : public streamer::Streamer {
 public:
  ~MockStreamer() override = default;

  MOCK_METHOD1(Frame, bool(cv::Mat& frame));
  MOCK_METHOD2(ChangeResolution, void(int x, int y));

  static bool FrameImpl(cv::Mat& Rect);
  //  static void ChangeResolutionImpl(int x, int y);
};

bool MockStreamer::FrameImpl(cv::Mat& frame) {
  const cv::Size size{800, 480};
  if (size != frame.size() || CV_8UC3 != frame.type()) {
    frame = cv::Mat{size, CV_8UC3};
  }
  return true;
}

// void MockStreamer::ChangeResolutionImpl(int, int) {
// Dummy now
//}

class MockVideoStatus : public VideoStatus {
 public:
  MockVideoStatus(){};
  MOCK_METHOD0(GetVideoStatus, bool());
};

class MatcherTest : public Test {
 protected:
  void SetUp() override;
  void TearDown() override;

  MockStreamer* streamer_ = nullptr;
  MockDetector<cv::Mat>* image_detector_ = nullptr;
  MockDetector<std::string>* text_detector_ = nullptr;
  MockVideoStatus* video_status_ = nullptr;

  std::unique_ptr<Matcher> matcher_;
  cv::Point tl_{0, 0};
  cv::Point br_{800, 480};
  static constexpr auto text_for_matching_ = "Any text";
};

void MatcherTest::SetUp() {
  std::unique_ptr<MockStreamer> streamer = std::make_unique<MockStreamer>();
  std::unique_ptr<MockDetector<cv::Mat>> image_detector = std::make_unique<MockDetector<cv::Mat>>();
  std::unique_ptr<MockDetector<std::string>> text_detector = std::make_unique<MockDetector<std::string>>();
  std::unique_ptr<MockVideoStatus> video_status = std::make_unique<MockVideoStatus>();

  streamer_ = streamer.get();
  image_detector_ = image_detector.get();
  text_detector_ = text_detector.get();
  video_status_ = video_status.get();

  matcher_ = std::make_unique<Matcher>(std::move(streamer), std::move(image_detector), std::move(text_detector),
                                       nullptr, std::move(video_status));

  ON_CALL(*video_status_, GetVideoStatus).WillByDefault(Return(true));
}

void MatcherTest::TearDown() {
  streamer_ = nullptr;
  image_detector_ = nullptr;
  text_detector_ = nullptr;

  matcher_.reset();
}

TEST_F(MatcherTest, DetectText_Success) {
  const unsigned frame_count = 2;
  const std::pair<cv::Rect, std::error_code> match_result = {{cv::Point{32, 32}, cv::Size{32, 32}}, std::error_code{}};

  EXPECT_CALL(*streamer_, Frame(_)).Times(frame_count).WillRepeatedly(Invoke(&MockStreamer::FrameImpl));
  EXPECT_CALL(*image_detector_, Detect(_, _, _)).Times(0);
  EXPECT_CALL(*text_detector_, Detect(_, text_for_matching_))
      .Times(frame_count)
      .WillRepeatedly(Return(match_result.first));

  for (unsigned i = 0; i < frame_count; ++i) {
    EXPECT_EQ(matcher_->DetectText(text_for_matching_, cv::Rect{}), match_result);
  }
}

TEST_F(MatcherTest, DetectImage_Success) {
  const unsigned frame_count = 2;
  const std::pair<cv::Rect, std::error_code> match_result = {{cv::Point{32, 32}, cv::Size{32, 32}}, std::error_code{}};

  constexpr auto object = "matcher_tests_small_image";
  const auto pattern = cv::imread(ATE_SERVER_TEST_DATA_PATH "/video_streaming/matching/matcher_tests_small_image.png");

  EXPECT_CALL(*streamer_, Frame(_)).Times(frame_count).WillRepeatedly(Invoke(&MockStreamer::FrameImpl));
  EXPECT_CALL(*image_detector_, Detect(_, _, _)).Times(frame_count).WillRepeatedly(Return(match_result.first));
  EXPECT_CALL(*text_detector_, Detect(_, _)).Times(0);

  for (unsigned i = 0; i < frame_count; ++i) {
    EXPECT_EQ(matcher_->DetectImage(object, pattern, cv::Rect{}), match_result);
  }
}

TEST_F(MatcherTest, DetectText_EmptyString_PatternInvalid) {
  EXPECT_CALL(*streamer_, Frame(_)).Times(0);
  EXPECT_CALL(*image_detector_, Detect(_, _, _)).Times(0);
  EXPECT_CALL(*text_detector_, Detect(_, {})).Times(0);

  const std::pair<cv::Rect, std::error_code> match_result = {
      cv::Rect{}, common::make_error_code(common::AteError::kPatternInvalid)};

  EXPECT_EQ(matcher_->DetectText({}, cv::Rect{}), match_result);
}

TEST_F(MatcherTest, DetectImage_TooBigPattern_PatternInvalid) {
  const unsigned frame_count = 2;

  constexpr auto object = "matcher_tests_big_image";
  const auto pattern = cv::imread(ATE_SERVER_TEST_DATA_PATH "/video_streaming/matching/matcher_tests_big_image.png");

  EXPECT_CALL(*streamer_, Frame(_)).Times(frame_count).WillRepeatedly(Invoke(&MockStreamer::FrameImpl));
  EXPECT_CALL(*image_detector_, Detect(_, _, _)).Times(0);
  EXPECT_CALL(*text_detector_, Detect(_, _)).Times(0);

  const std::pair<cv::Rect, std::error_code> match_result = {
      cv::Rect{}, common::make_error_code(common::AteError::kPatternInvalid)};

  for (unsigned i = 0; i < frame_count; ++i) {
    EXPECT_EQ(matcher_->DetectImage(object, pattern, cv::Rect{}), match_result);
  }
}

TEST_F(MatcherTest, DetectImage_PatternBiggerThenSearchRegion_PatternInvalid) {
  constexpr auto object = "matcher_tests_small_image";
  const auto pattern = cv::imread(ATE_SERVER_TEST_DATA_PATH "/video_streaming/matching/matcher_tests_small_image.png");

  EXPECT_CALL(*streamer_, Frame(_)).Times(1).WillRepeatedly(Invoke(&MockStreamer::FrameImpl));
  EXPECT_CALL(*image_detector_, Detect(_, _, _)).Times(0);
  EXPECT_CALL(*text_detector_, Detect(_, _)).Times(0);

  const std::pair<cv::Rect, std::error_code> match_result = {
      cv::Rect{}, common::make_error_code(common::AteError::kPatternInvalid)};

  EXPECT_EQ(matcher_->DetectImage(object, pattern, cv::Rect{cv::Point{0, 0}, cv::Point{10, 10}}), match_result);
}

TEST_F(MatcherTest, DetectText_IncorrectCoordinates_BottomRightXOutOfBoundaries) {
  cv::Point br_x_out_boundaries{801, 480};

  EXPECT_CALL(*video_status_, GetVideoStatus()).WillRepeatedly(Return(true));
  EXPECT_CALL(*streamer_, Frame(_)).WillOnce(Invoke(&MockStreamer::FrameImpl));
  EXPECT_CALL(*text_detector_, Detect(_, _)).Times(0);

  const std::pair<cv::Rect, std::error_code> match_result = {
      cv::Rect{}, common::make_error_code(common::AteError::kOutOfBoundaries)};

  EXPECT_EQ(matcher_->DetectText(text_for_matching_, cv::Rect{tl_, br_x_out_boundaries}), match_result);
}

TEST_F(MatcherTest, DetectText_IncorrectCoordinates_BottomRightYOutOfBoundaries) {
  cv::Point br_y_out_boundaries{800, 481};

  EXPECT_CALL(*video_status_, GetVideoStatus()).WillRepeatedly(Return(true));
  EXPECT_CALL(*streamer_, Frame(_)).WillOnce(Invoke(&MockStreamer::FrameImpl));
  EXPECT_CALL(*text_detector_, Detect(_, _)).Times(0);

  const std::pair<cv::Rect, std::error_code> match_result = {
      cv::Rect{}, common::make_error_code(common::AteError::kOutOfBoundaries)};

  EXPECT_EQ(matcher_->DetectText(text_for_matching_, cv::Rect{tl_, br_y_out_boundaries}), match_result);
}

TEST_F(MatcherTest, DetectText_IncorrectCoordinates_TopLeftXOutOfBoundaries) {
  cv::Point tl_x_out_boundaries{-1, 0};

  EXPECT_CALL(*video_status_, GetVideoStatus()).WillRepeatedly(Return(true));
  EXPECT_CALL(*streamer_, Frame(_)).WillOnce(Invoke(&MockStreamer::FrameImpl));
  EXPECT_CALL(*text_detector_, Detect(_, _)).Times(0);

  const std::pair<cv::Rect, std::error_code> match_result = {
      cv::Rect{}, common::make_error_code(common::AteError::kOutOfBoundaries)};

  EXPECT_EQ(matcher_->DetectText(text_for_matching_, cv::Rect{tl_x_out_boundaries, br_}), match_result);
}

TEST_F(MatcherTest, DetectText_IncorrectCoordinates_TopLeftYOutOfBoundaries) {
  cv::Point tl_y_out_boundaries{0, -1};

  EXPECT_CALL(*video_status_, GetVideoStatus()).WillRepeatedly(Return(true));
  EXPECT_CALL(*streamer_, Frame(_)).WillOnce(Invoke(&MockStreamer::FrameImpl));
  EXPECT_CALL(*text_detector_, Detect(_, _)).Times(0);

  const std::pair<cv::Rect, std::error_code> match_result = {
      cv::Rect{}, common::make_error_code(common::AteError::kOutOfBoundaries)};

  EXPECT_EQ(matcher_->DetectText(text_for_matching_, cv::Rect{tl_y_out_boundaries, br_}), match_result);
}

TEST_F(MatcherTest, DetectImage_IncorrectCoordinates_OutOfBoundaries) {
  cv::Point br_x_out_boundaries{801, 480};

  EXPECT_CALL(*video_status_, GetVideoStatus()).WillRepeatedly(Return(true));
  EXPECT_CALL(*streamer_, Frame(_)).WillOnce(Invoke(&MockStreamer::FrameImpl));
  EXPECT_CALL(*image_detector_, Detect(_, _, _)).Times(0);

  const std::pair<cv::Rect, std::error_code> match_result = {
      cv::Rect{}, common::make_error_code(common::AteError::kOutOfBoundaries)};

  EXPECT_EQ(matcher_->DetectImage("icon_object", cv::Mat{200, 200}, cv::Rect{tl_, br_x_out_boundaries}), match_result);
}

TEST_F(MatcherTest, DetectText_TextNotRecognized_PatternNotFound) {
  EXPECT_CALL(*streamer_, Frame(_)).WillOnce(Invoke(&MockStreamer::FrameImpl));
  EXPECT_CALL(*image_detector_, Detect(_, _, _)).Times(0);
  EXPECT_CALL(*text_detector_, Detect(_, text_for_matching_)).WillOnce(Return(cv::Rect{}));

  const std::pair<cv::Rect, std::error_code> match_result = {
      cv::Rect{}, common::make_error_code(common::AteError::kPatternNotFound)};

  EXPECT_EQ(matcher_->DetectText(text_for_matching_, cv::Rect{}), match_result);
}

TEST_F(MatcherTest, Matching_StreamingServiceOff_VideoUnavailable) {
  constexpr auto object = "matcher_tests_small_image";
  const auto pattern = cv::imread(ATE_SERVER_TEST_DATA_PATH "/video_streaming/matching/matcher_tests_small_image.png");

  EXPECT_CALL(*streamer_, Frame(_)).WillRepeatedly(Return(false));
  EXPECT_CALL(*image_detector_, Detect(_, _, _)).Times(0);
  EXPECT_CALL(*text_detector_, Detect(_, text_for_matching_)).Times(0);

  const std::pair<cv::Rect, std::error_code> match_result = {
      cv::Rect{}, common::make_error_code(common::AteError::kVideoTemporarilyUnavailable)};

  EXPECT_EQ(matcher_->DetectImage(object, pattern, cv::Rect{}), match_result);
  EXPECT_EQ(matcher_->DetectText(text_for_matching_, cv::Rect{}), match_result);
}

TEST_F(MatcherTest, Matching_VideoStatusOff_VideoUnavailable) {
  constexpr auto object = "matcher_tests_small_image";
  const auto pattern = cv::imread(ATE_SERVER_TEST_DATA_PATH "/video_streaming/matching/matcher_tests_small_image.png");

  EXPECT_CALL(*video_status_, GetVideoStatus()).WillRepeatedly(Return(false));
  EXPECT_CALL(*streamer_, Frame(_)).Times(0);
  EXPECT_CALL(*image_detector_, Detect(_, _, _)).Times(0);
  EXPECT_CALL(*text_detector_, Detect(_, text_for_matching_)).Times(0);

  const std::pair<cv::Rect, std::error_code> match_result = {
      cv::Rect{}, common::make_error_code(common::AteError::kVideoTemporarilyUnavailable)};

  EXPECT_EQ(matcher_->DetectImage(object, pattern, cv::Rect{}), match_result);
  EXPECT_EQ(matcher_->DetectText(text_for_matching_, cv::Rect{}), match_result);
}

TEST_F(MatcherTest, GetText_VideoStatusOff_VideoUnvailable) {
  EXPECT_CALL(*video_status_, GetVideoStatus()).WillRepeatedly(Return(false));
  EXPECT_CALL(*streamer_, Frame(_)).Times(0);

  const std::pair<std::string, std::error_code> kResult{
      {}, common::make_error_code(common::AteError::kVideoTemporarilyUnavailable)};

  EXPECT_EQ(kResult, matcher_->GetText(cv::Point{10, 20}, cv::Point{70, 80}));
}

TEST_F(MatcherTest, GetText_GrabNewFrameFail_VideoUnvailable) {
  EXPECT_CALL(*video_status_, GetVideoStatus()).WillRepeatedly(Return(true));
  EXPECT_CALL(*streamer_, Frame(_)).WillRepeatedly(Return(false));

  const std::pair<std::string, std::error_code> kResult{
      {}, common::make_error_code(common::AteError::kVideoTemporarilyUnavailable)};

  EXPECT_EQ(kResult, matcher_->GetText(cv::Point{10, 20}, cv::Point{70, 80}));
}

TEST_F(MatcherTest, GetText_IncorrectCoordinates_OutOfBoundaries) {
  EXPECT_CALL(*video_status_, GetVideoStatus()).WillRepeatedly(Return(true));
  EXPECT_CALL(*streamer_, Frame(_)).Times(1).WillRepeatedly(Invoke(&MockStreamer::FrameImpl));

  const std::pair<std::string, std::error_code> kResult{{},
                                                        common::make_error_code(common::AteError::kOutOfBoundaries)};

  EXPECT_EQ(kResult, matcher_->GetText(cv::Point{100000, 2000000}, cv::Point{9999999, 99999999}));
}

TEST_F(MatcherTest, GetText_ValidCoordinates_RecognizedText) {
  EXPECT_CALL(*video_status_, GetVideoStatus()).WillRepeatedly(Return(true));
  EXPECT_CALL(*streamer_, Frame(_)).Times(1).WillRepeatedly(Invoke([](cv::Mat& frame) {
    frame = cv::imread(ATE_SERVER_TEST_DATA_PATH "/video_streaming/matching/matcher_tests_big_image.png");
    return true;
  }));

  EXPECT_CALL(*text_detector_, ExtractText(_)).Times(1).WillRepeatedly(Return("Recognized text"));

  const std::pair<std::string, std::error_code> kResult{"Recognized text", {}};
  const auto recognized = matcher_->GetText(cv::Point{0, 0}, cv::Point{10, 20});

  EXPECT_EQ(kResult, recognized);
}

TEST_F(MatcherTest, GetScreenshot_WrongExtension_WrongExtensionError) {
  constexpr auto wrong_extension_filename = "filename.wrong";
  constexpr auto path = "path";

  EXPECT_CALL(*video_status_, GetVideoStatus()).WillRepeatedly(Return(true));
  EXPECT_CALL(*streamer_, Frame(_)).WillOnce(Invoke(&MockStreamer::FrameImpl));

  auto expected_error_code = common::make_error_code(common::AteError::kWrongExtension);
  auto error_code = matcher_->GetScreenshot(wrong_extension_filename, path);

  EXPECT_EQ(error_code, expected_error_code)
      << "Expected error: " << expected_error_code.message() << " Returned error: " << error_code.message();
}

TEST_F(MatcherTest, GetScreenshot_EmptyFileNameErrorCode_EmptyFileNameError) {
  constexpr auto empty_filename = "";
  constexpr auto empty_path = "";

  EXPECT_CALL(*video_status_, GetVideoStatus()).WillRepeatedly(Return(true));
  EXPECT_CALL(*streamer_, Frame(_)).WillOnce(Invoke(&MockStreamer::FrameImpl));

  auto expected_error_code = common::make_error_code(common::AteError::kEmptyFileName);
  auto error_code = matcher_->GetScreenshot(empty_filename, empty_path);

  EXPECT_EQ(error_code, expected_error_code)
      << "Expected error: " << expected_error_code.message() << " Returned error: " << error_code.message();
}

TEST_F(MatcherTest, GetScreenshot_VideoTemporarilyUnavailableErrorCode_VideoTemporarilyUnavailableError) {
  constexpr auto filename = "file.png";
  constexpr auto path = "screenshot";

  EXPECT_CALL(*video_status_, GetVideoStatus()).WillRepeatedly(Return(false));

  auto expected_error_code = common::make_error_code(common::AteError::kVideoTemporarilyUnavailable);
  auto error_code = matcher_->GetScreenshot(filename, path);

  EXPECT_EQ(error_code, expected_error_code)
      << "Expected error: " << expected_error_code.message() << " Returned error: " << error_code.message();
}

TEST_F(MatcherTest, GetImagesDiscrepancy_DifferentImagesResolution_WrongImageResolution) {
  const auto path_to_screenshot1 = ATE_SERVER_TEST_DATA_PATH "/video_streaming/matching/matcher_tests_big_image.png";
  const auto path_to_screenshot2 = ATE_SERVER_TEST_DATA_PATH "/video_streaming/matching/matcher_tests_small_image.png";

  auto result = matcher_->GetImagesDiscrepancy(path_to_screenshot1, path_to_screenshot2, {}, {});
  EXPECT_EQ(result.second, common::AteError::kWrongImageResolution);
}

TEST_F(MatcherTest, GetImagesDiscrepancy_InvalidCoordinates_OutOfBoundaries) {
  const auto path_to_screenshot = ATE_SERVER_TEST_DATA_PATH "/video_streaming/matching/matcher_tests_big_image.png";

  auto result = matcher_->GetImagesDiscrepancy(path_to_screenshot, path_to_screenshot, {0, 0}, {0, 99999});
  EXPECT_EQ(result.second, common::AteError::kOutOfBoundaries);
}

TEST_F(MatcherTest, GetImagesDiscrepancy_InvalidImages_SystemError) {
  const auto path_to_screenshot = "invalid_path";

  auto result = matcher_->GetImagesDiscrepancy(path_to_screenshot, path_to_screenshot, {0, 0}, {0, 0});
  EXPECT_EQ(result.second, common::AteError::kSystemError);
}

TEST_F(MatcherTest, GetImagesDiscrepancy_ValidData_NoError) {
  const auto path_to_screenshot = ATE_SERVER_TEST_DATA_PATH "/video_streaming/matching/matcher_tests_big_image.png";

  auto result = matcher_->GetImagesDiscrepancy(path_to_screenshot, path_to_screenshot, {0, 0}, {0, 0});
  EXPECT_EQ(result.second, std::error_code{});
  EXPECT_EQ(result.first, 0);

  result = matcher_->GetImagesDiscrepancy(path_to_screenshot, path_to_screenshot, {0, 0}, {100, 100});
  EXPECT_EQ(result.second, std::error_code{});
  EXPECT_EQ(result.first, 0);
}

TEST_F(MatcherTest, DetectImages_TwoCall_DetectImagesInvokedTwice) {
  const unsigned frame_count = 2;
  const auto pattern = cv::imread(ATE_SERVER_TEST_DATA_PATH "/video_streaming/matching/matcher_tests_small_image.png");

  EXPECT_CALL(*streamer_, Frame(_)).Times(frame_count).WillRepeatedly(Invoke(&MockStreamer::FrameImpl));
  EXPECT_CALL(*image_detector_, DetectAll(_, _, _)).Times(frame_count);
  EXPECT_CALL(*image_detector_, Detect(_, _, _)).Times(0);
  EXPECT_CALL(*text_detector_, Detect(_, _)).Times(0);

  for (unsigned i = 0; i < frame_count; ++i) {
    matcher_->DetectImages("", pattern);
  }
}

TEST_F(MatcherTest, DetectImages_NotExistPattern_PatternNotFound) {
  const std::vector<cv::Rect> empty_vector;
  const std::pair<std::vector<cv::Rect>, std::error_code> match_result = {
      std::vector<cv::Rect>{}, common::make_error_code(common::AteError::kPatternNotFound)};
  const auto pattern = cv::imread(ATE_SERVER_TEST_DATA_PATH "/video_streaming/matching/matcher_tests_small_image.png");

  EXPECT_CALL(*streamer_, Frame(_)).Times(1).WillRepeatedly(Invoke(&MockStreamer::FrameImpl));
  EXPECT_CALL(*image_detector_, DetectAll(_, _, _)).Times(1).WillRepeatedly(Return(empty_vector));
  EXPECT_CALL(*image_detector_, Detect(_, _, _)).Times(0);
  EXPECT_CALL(*text_detector_, Detect(_, _)).Times(0);

  EXPECT_EQ(matcher_->DetectImages("", pattern), match_result);
}

TEST_F(MatcherTest, DetectImages_OneObject_OneObject) {
  const std::vector<cv::Rect> result_vector{cv::Rect()};
  const std::pair<std::vector<cv::Rect>, std::error_code> match_result = {result_vector, {}};
  const auto pattern = cv::imread(ATE_SERVER_TEST_DATA_PATH "/video_streaming/matching/matcher_tests_small_image.png");

  EXPECT_CALL(*streamer_, Frame(_)).Times(1).WillRepeatedly(Invoke(&MockStreamer::FrameImpl));
  EXPECT_CALL(*image_detector_, DetectAll(_, _, _)).Times(1).WillRepeatedly(Return(result_vector));
  EXPECT_CALL(*image_detector_, Detect(_, _, _)).Times(0);
  EXPECT_CALL(*text_detector_, Detect(_, _)).Times(0);

  EXPECT_EQ(matcher_->DetectImages("", pattern), match_result);
}

TEST_F(MatcherTest, DetectImages_VideoStatusError_VideoTemporarilyUnavailable) {
  const auto pattern = cv::imread(ATE_SERVER_TEST_DATA_PATH "/video_streaming/matching/matcher_tests_small_image.png");
  const std::pair<std::vector<cv::Rect>, std::error_code> match_result = {
      std::vector<cv::Rect>{}, common::make_error_code(common::AteError::kVideoTemporarilyUnavailable)};
  ON_CALL(*video_status_, GetVideoStatus).WillByDefault(Return(false));

  EXPECT_CALL(*image_detector_, DetectAll(_, _, _)).Times(0);
  EXPECT_CALL(*image_detector_, Detect(_, _, _)).Times(0);
  EXPECT_CALL(*text_detector_, Detect(_, _)).Times(0);

  EXPECT_EQ(matcher_->DetectImages("", pattern), match_result);
}

TEST_F(MatcherTest, DetectImages_TooBigPattern_PatternInvalid) {
  const auto pattern = cv::imread(ATE_SERVER_TEST_DATA_PATH "/video_streaming/matching/matcher_tests_big_image.png");
  const std::pair<std::vector<cv::Rect>, std::error_code> match_result = {
      std::vector<cv::Rect>{}, common::make_error_code(common::AteError::kPatternInvalid)};

  EXPECT_CALL(*streamer_, Frame(_)).Times(1).WillRepeatedly(Invoke(&MockStreamer::FrameImpl));
  EXPECT_CALL(*image_detector_, DetectAll(_, _, _)).Times(0);
  EXPECT_CALL(*image_detector_, Detect(_, _, _)).Times(0);
  EXPECT_CALL(*text_detector_, Detect(_, _)).Times(0);

  EXPECT_EQ(matcher_->DetectImages("", pattern), match_result);
}

TEST_F(MatcherTest, DetectImages_EmptyPattern_PatternInvalid) {
  cv::Mat pattern;
  const std::pair<std::vector<cv::Rect>, std::error_code> match_result = {
      std::vector<cv::Rect>{}, common::make_error_code(common::AteError::kPatternInvalid)};

  EXPECT_CALL(*streamer_, Frame(_)).Times(1).WillRepeatedly(Invoke(&MockStreamer::FrameImpl));
  EXPECT_CALL(*image_detector_, DetectAll(_, _, _)).Times(0);
  EXPECT_CALL(*image_detector_, Detect(_, _, _)).Times(0);
  EXPECT_CALL(*text_detector_, Detect(_, _)).Times(0);

  EXPECT_EQ(matcher_->DetectImages("", pattern), match_result);
}

TEST_F(MatcherTest, DetectImages_ValidSearchArea_DetectAllInvokedOnce) {
  cv::Rect search_area(0, 0, 100, 100);
  const auto pattern = cv::imread(ATE_SERVER_TEST_DATA_PATH "/video_streaming/matching/matcher_tests_small_image.png");

  EXPECT_CALL(*streamer_, Frame(_)).Times(1).WillRepeatedly(Invoke(&MockStreamer::FrameImpl));
  EXPECT_CALL(*image_detector_, DetectAll(_, _, _)).Times(1);
  EXPECT_CALL(*image_detector_, Detect(_, _, _)).Times(0);
  EXPECT_CALL(*text_detector_, Detect(_, _)).Times(0);

  matcher_->DetectImages("", pattern, search_area);
}

TEST_F(MatcherTest, DetectImages_SearchAreaBiggerThanImage_OutOfBoundaries) {
  cv::Rect search_area(0, 0, 2000, 100);
  const auto pattern = cv::imread(ATE_SERVER_TEST_DATA_PATH "/video_streaming/matching/matcher_tests_small_image.png");
  const std::pair<std::vector<cv::Rect>, std::error_code> match_result = {
      std::vector<cv::Rect>{}, common::make_error_code(common::AteError::kOutOfBoundaries)};

  EXPECT_CALL(*streamer_, Frame(_)).Times(1).WillRepeatedly(Invoke(&MockStreamer::FrameImpl));
  EXPECT_CALL(*image_detector_, DetectAll(_, _, _)).Times(0);
  EXPECT_CALL(*image_detector_, Detect(_, _, _)).Times(0);
  EXPECT_CALL(*text_detector_, Detect(_, _)).Times(0);

  EXPECT_EQ(matcher_->DetectImages("", pattern, search_area), match_result);
}

TEST_F(MatcherTest, DetectImages_SearchAreaLessThanPattern_PatternInvalid) {
  cv::Rect search_area(0, 0, 1, 2);
  const auto pattern = cv::imread(ATE_SERVER_TEST_DATA_PATH "/video_streaming/matching/matcher_tests_small_image.png");
  const std::pair<std::vector<cv::Rect>, std::error_code> match_result = {
      std::vector<cv::Rect>{}, common::make_error_code(common::AteError::kPatternInvalid)};

  EXPECT_CALL(*streamer_, Frame(_)).Times(1).WillRepeatedly(Invoke(&MockStreamer::FrameImpl));
  EXPECT_CALL(*image_detector_, DetectAll(_, _, _)).Times(0);
  EXPECT_CALL(*image_detector_, Detect(_, _, _)).Times(0);
  EXPECT_CALL(*text_detector_, Detect(_, _)).Times(0);

  EXPECT_EQ(matcher_->DetectImages("", pattern, search_area), match_result);
}

}  // namespace
