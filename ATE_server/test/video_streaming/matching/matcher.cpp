#include "video_streaming/matching/matcher.h"

#include <memory>
#include <string>
#include <utility>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/imgcodecs.hpp>

#include "video_streaming/matching/detector.h"
#include "video_streaming/streamer.h"

namespace {

using ::testing::_;
using ::testing::Invoke;
using ::testing::Return;
using ::testing::Test;

using namespace detector;

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

  static bool FrameImpl(cv::Mat& Rect);
};

bool MockStreamer::FrameImpl(cv::Mat& frame) {
  const cv::Size size{800, 480};
  if (size != frame.size() || CV_8UC3 != frame.type()) {
    frame = cv::Mat{size, CV_8UC3};
  }
  return true;
}

class MatcherTest : public Test {
 protected:
  void SetUp() override;
  void TearDown() override;

  MockStreamer* streamer_ = nullptr;
  MockDetector<cv::Mat>* image_detector_ = nullptr;
  MockDetector<std::string>* text_detector_ = nullptr;

  std::unique_ptr<Matcher> matcher_;
};

void MatcherTest::SetUp() {
  std::unique_ptr<MockStreamer> streamer = std::make_unique<MockStreamer>();
  std::unique_ptr<MockDetector<cv::Mat>> image_detector = std::make_unique<MockDetector<cv::Mat>>();
  std::unique_ptr<MockDetector<std::string>> text_detector = std::make_unique<MockDetector<std::string>>();

  streamer_ = streamer.get();
  image_detector_ = image_detector.get();
  text_detector_ = text_detector.get();

  matcher_ =
      std::make_unique<Matcher>(std::move(streamer), std::move(image_detector), std::move(text_detector), nullptr);
}

void MatcherTest::TearDown() {
  streamer_ = nullptr;
  image_detector_ = nullptr;
  text_detector_ = nullptr;

  matcher_.reset();
}

TEST_F(MatcherTest, MatchText_Success) {
  const unsigned frame_count = 2;
  const cv::Rect result_rect{cv::Point{32, 32}, cv::Size{32, 32}};

  constexpr auto text_for_matching = "Any text";

  EXPECT_CALL(*streamer_, Frame(_)).Times(frame_count).WillRepeatedly(Invoke(&MockStreamer::FrameImpl));
  EXPECT_CALL(*image_detector_, Detect(_, _)).Times(0);
  EXPECT_CALL(*text_detector_, Detect(_, text_for_matching)).Times(frame_count).WillRepeatedly(Return(result_rect));

  for (unsigned i = 0; i < frame_count; ++i) {
    EXPECT_EQ(matcher_->MatchText(text_for_matching), result_rect);
  }
}

TEST_F(MatcherTest, MatchImage_Success) {
  const unsigned frame_count = 2;
  const cv::Rect result_rect{cv::Point{32, 32}, cv::Size{32, 32}};

  constexpr auto object = "matcher_tests_small_image";
  const auto pattern = cv::imread(VHAT_SERVER_TEST_DATA_PATH "/video_streaming/matching/matcher_tests_small_image.png");

  EXPECT_CALL(*streamer_, Frame(_)).Times(frame_count).WillRepeatedly(Invoke(&MockStreamer::FrameImpl));
  EXPECT_CALL(*image_detector_, Detect(_, _)).Times(frame_count).WillRepeatedly(Return(result_rect));
  EXPECT_CALL(*text_detector_, Detect(_, _)).Times(0);

  for (unsigned i = 0; i < frame_count; ++i) {
    EXPECT_EQ(matcher_->MatchImage(object, pattern), result_rect);
  }
}

TEST_F(MatcherTest, MatchText_EmptyString_Failure) {
  EXPECT_CALL(*streamer_, Frame(_)).Times(0);
  EXPECT_CALL(*image_detector_, Detect(_, _)).Times(0);
  EXPECT_CALL(*text_detector_, Detect(_, {})).Times(0);

  EXPECT_TRUE(matcher_->MatchText({}).empty());
}

TEST_F(MatcherTest, MatchImage_TooBigPattern_Failure) {
  const unsigned frame_count = 2;

  constexpr auto object = "matcher_tests_big_image";
  const auto pattern = cv::imread(VHAT_SERVER_TEST_DATA_PATH "/video_streaming/matching/matcher_tests_big_image.png");

  EXPECT_CALL(*streamer_, Frame(_)).Times(frame_count).WillRepeatedly(Invoke(&MockStreamer::FrameImpl));
  EXPECT_CALL(*image_detector_, Detect(_, _)).Times(0);
  EXPECT_CALL(*text_detector_, Detect(_, _)).Times(0);

  for (unsigned i = 0; i < frame_count; ++i) {
    EXPECT_TRUE(matcher_->MatchImage(object, pattern).empty());
  }
}

TEST_F(MatcherTest, InvalidStream_Failure) {
  constexpr auto object = "matcher_tests_small_image";
  constexpr auto text_for_matching = "Any text";
  const auto pattern = cv::imread(VHAT_SERVER_TEST_DATA_PATH "/video_streaming/matching/matcher_tests_small_image.png");

  EXPECT_CALL(*streamer_, Frame(_)).WillRepeatedly(Return(false));
  EXPECT_CALL(*image_detector_, Detect(_, _)).Times(0);
  EXPECT_CALL(*text_detector_, Detect(_, text_for_matching)).Times(0);

  EXPECT_TRUE(matcher_->MatchImage(object, pattern).empty());
  EXPECT_TRUE(matcher_->MatchText(text_for_matching).empty());
}

}  // namespace
