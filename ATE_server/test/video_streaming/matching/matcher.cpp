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

  MOCK_METHOD2_T(Detect, cv::Rect(const cv::Mat& frame, const T& pattern));
};

class MockStreamer : public streamer::Streamer {
 public:
  ~MockStreamer() override = default;

  MOCK_METHOD1(Frame, bool(cv::Mat& frame));
  MOCK_METHOD2(ChangeResolution, void(int x, int y));

  static bool FrameImpl(cv::Mat& Rect);
  static void ChangeResolutionImpl(int x, int y);
};

bool MockStreamer::FrameImpl(cv::Mat& frame) {
  const cv::Size size{800, 480};
  if (size != frame.size() || CV_8UC3 != frame.type()) {
    frame = cv::Mat{size, CV_8UC3};
  }
  return true;
}

void MockStreamer::ChangeResolutionImpl(int, int) {
  // Dummy now
}

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

TEST_F(MatcherTest, MatchText_Success) {
  const unsigned frame_count = 2;
  const std::pair<cv::Rect, std::error_code> match_result = {{cv::Point{32, 32}, cv::Size{32, 32}}, std::error_code{}};
  constexpr auto text_for_matching = "Any text";

  EXPECT_CALL(*streamer_, Frame(_)).Times(frame_count).WillRepeatedly(Invoke(&MockStreamer::FrameImpl));
  EXPECT_CALL(*image_detector_, Detect(_, _)).Times(0);
  EXPECT_CALL(*text_detector_, Detect(_, text_for_matching))
      .Times(frame_count)
      .WillRepeatedly(Return(match_result.first));

  for (unsigned i = 0; i < frame_count; ++i) {
    EXPECT_EQ(matcher_->MatchText(text_for_matching), match_result);
  }
}

TEST_F(MatcherTest, MatchImage_Success) {
  const unsigned frame_count = 2;
  const std::pair<cv::Rect, std::error_code> match_result = {{cv::Point{32, 32}, cv::Size{32, 32}}, std::error_code{}};

  constexpr auto object = "matcher_tests_small_image";
  const auto pattern = cv::imread(ATE_SERVER_TEST_DATA_PATH "/video_streaming/matching/matcher_tests_small_image.png");

  EXPECT_CALL(*streamer_, Frame(_)).Times(frame_count).WillRepeatedly(Invoke(&MockStreamer::FrameImpl));
  EXPECT_CALL(*image_detector_, Detect(_, _)).Times(frame_count).WillRepeatedly(Return(match_result.first));
  EXPECT_CALL(*text_detector_, Detect(_, _)).Times(0);

  for (unsigned i = 0; i < frame_count; ++i) {
    EXPECT_EQ(matcher_->MatchImage(object, pattern), match_result);
  }
}

TEST_F(MatcherTest, MatchText_EmptyString_PatternInvalid) {
  EXPECT_CALL(*streamer_, Frame(_)).Times(0);
  EXPECT_CALL(*image_detector_, Detect(_, _)).Times(0);
  EXPECT_CALL(*text_detector_, Detect(_, {})).Times(0);

  const std::pair<cv::Rect, std::error_code> match_result = {
      cv::Rect{}, common::make_error_code(common::AteError::kPatternInvalid)};

  EXPECT_EQ(matcher_->MatchText({}), match_result);
}

TEST_F(MatcherTest, MatchImage_TooBigPattern_PatternInvalid) {
  const unsigned frame_count = 2;

  constexpr auto object = "matcher_tests_big_image";
  const auto pattern = cv::imread(ATE_SERVER_TEST_DATA_PATH "/video_streaming/matching/matcher_tests_big_image.png");

  EXPECT_CALL(*streamer_, Frame(_)).Times(frame_count).WillRepeatedly(Invoke(&MockStreamer::FrameImpl));
  EXPECT_CALL(*image_detector_, Detect(_, _)).Times(0);
  EXPECT_CALL(*text_detector_, Detect(_, _)).Times(0);

  const std::pair<cv::Rect, std::error_code> match_result = {
      cv::Rect{}, common::make_error_code(common::AteError::kPatternInvalid)};

  for (unsigned i = 0; i < frame_count; ++i) {
    EXPECT_EQ(matcher_->MatchImage(object, pattern), match_result);
  }
}

TEST_F(MatcherTest, MatchText_TextNotRecognized_PatternNotFound) {
  constexpr auto text_for_matching = "Any text";

  EXPECT_CALL(*streamer_, Frame(_)).WillOnce(Invoke(&MockStreamer::FrameImpl));
  EXPECT_CALL(*image_detector_, Detect(_, _)).Times(0);
  EXPECT_CALL(*text_detector_, Detect(_, text_for_matching)).WillOnce(Return(cv::Rect{}));

  const std::pair<cv::Rect, std::error_code> match_result = {
      cv::Rect{}, common::make_error_code(common::AteError::kPatternNotFound)};

  EXPECT_EQ(matcher_->MatchText(text_for_matching), match_result);
}

TEST_F(MatcherTest, StreamingServiceOff_VideoUnavailable) {
  constexpr auto object = "matcher_tests_small_image";
  constexpr auto text_for_matching = "Any text";
  const auto pattern = cv::imread(ATE_SERVER_TEST_DATA_PATH "/video_streaming/matching/matcher_tests_small_image.png");

  EXPECT_CALL(*streamer_, Frame(_)).WillRepeatedly(Return(false));
  EXPECT_CALL(*image_detector_, Detect(_, _)).Times(0);
  EXPECT_CALL(*text_detector_, Detect(_, text_for_matching)).Times(0);

  const std::pair<cv::Rect, std::error_code> match_result = {
      cv::Rect{}, common::make_error_code(common::AteError::kVideoTemporarilyUnavailable)};

  EXPECT_EQ(matcher_->MatchImage(object, pattern), match_result);
  EXPECT_EQ(matcher_->MatchText(text_for_matching), match_result);
}

TEST_F(MatcherTest, VideoStatusOff_VideoUnavailable) {
  constexpr auto object = "matcher_tests_small_image";
  constexpr auto text_for_matching = "Any text";
  const auto pattern = cv::imread(ATE_SERVER_TEST_DATA_PATH "/video_streaming/matching/matcher_tests_small_image.png");

  EXPECT_CALL(*video_status_, GetVideoStatus()).WillRepeatedly(Return(false));
  EXPECT_CALL(*streamer_, Frame(_)).Times(0);
  EXPECT_CALL(*image_detector_, Detect(_, _)).Times(0);
  EXPECT_CALL(*text_detector_, Detect(_, text_for_matching)).Times(0);

  const std::pair<cv::Rect, std::error_code> match_result = {
      cv::Rect{}, common::make_error_code(common::AteError::kVideoTemporarilyUnavailable)};

  EXPECT_EQ(matcher_->MatchImage(object, pattern), match_result);
  EXPECT_EQ(matcher_->MatchText(text_for_matching), match_result);
}

}  // namespace
