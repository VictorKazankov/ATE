#include <gtest/gtest.h>

#include <opencv2/imgcodecs.hpp>

#include "video_streaming/matching/template_detector.h"

namespace {

// Path to icons
constexpr auto kFramePath = VHAT_SERVER_TEST_DATA_PATH "/video_streaming/matching/hmi_screenshot.png";
constexpr auto kHomeIcon = VHAT_SERVER_TEST_DATA_PATH "/video_streaming/matching/home.png";
constexpr auto kBadIcon = VHAT_SERVER_TEST_DATA_PATH "/video_streaming/matching/bad_icon.png";

// Start point (x, y) and (width, height) of home button
const cv::Rect home_rect{44,2,50,45};

class TemplateDetectorTest : public ::testing::Test {
 protected:
  void SetUp() override;
  void TearDown() override;

  std::unique_ptr<detector::TemplateDetector> template_detector_;
  cv::Mat frame_;
};

void TemplateDetectorTest::SetUp() {
  template_detector_ = std::make_unique<detector::TemplateDetector>();
  frame_ = cv::imread(kFramePath, cv::IMREAD_GRAYSCALE);
}
void TemplateDetectorTest::TearDown() { template_detector_.reset(); }

TEST_F(TemplateDetectorTest, BadDetectionVerify) {
  auto pattern = cv::imread(kBadIcon, cv::IMREAD_GRAYSCALE);
  auto detected = template_detector_->Detect(frame_, pattern);
  EXPECT_FALSE(detected.area());
}

TEST_F(TemplateDetectorTest, GoodDetectionVerify) {
  auto pattern = cv::imread(kHomeIcon, cv::IMREAD_GRAYSCALE);
  auto detected = template_detector_->Detect(frame_, pattern);
  EXPECT_TRUE(detected.area());
}

TEST_F(TemplateDetectorTest, GoodDetecting) {
  auto pattern = cv::imread(kHomeIcon, cv::IMREAD_GRAYSCALE);
  const auto detect_area = template_detector_->Detect(frame_, pattern);

  EXPECT_EQ(home_rect, detect_area);
}
}  // namespace
