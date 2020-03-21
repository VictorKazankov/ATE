#include <experimental/filesystem>
#include <fstream>
#include <thread>

#include <gtest/gtest.h>

#include "utils/video_status.h"

namespace {
namespace fs = std::experimental::filesystem;
using namespace utils;
using ::testing::Test;

class VideoStatusTest : public Test {
 protected:
  void SetUp() override;
  void TearDown() override;

  void TurnOn() { std::ofstream(gpio_path_ / kValue) << "1"; }
  void TurnOff() { std::ofstream(gpio_path_ / kValue) << "0"; }
  const fs::path gpio_path_ = "/tmp/gpio";
  const std::string kValue = "value";
  std::unique_ptr<GpioVideoStatus> video_status_;
};

void VideoStatusTest::SetUp() {
  fs::create_directories(gpio_path_);
  TurnOff();
  video_status_ = std::make_unique<GpioVideoStatus>(gpio_path_);
}
void VideoStatusTest::TearDown() {
  fs::remove_all(gpio_path_);
  video_status_.reset();
}

TEST_F(VideoStatusTest, GetVideoStatus_GpioUnvailable_Success) {
  GpioVideoStatus video_health_without_gpio("");
  EXPECT_TRUE(video_health_without_gpio.GetVideoStatus()) << "Status should be on in case gpio not supported";
}

TEST_F(VideoStatusTest, GetVideoStatus_GpioOff_Failure) {
  TurnOff();
  EXPECT_FALSE(video_status_->GetVideoStatus()) << "Status should be off";
}

TEST_F(VideoStatusTest, GetVideoStatus_GpioOn_Success) {
  TurnOn();
  EXPECT_TRUE(video_status_->GetVideoStatus()) << "Status should be on";
}

TEST_F(VideoStatusTest, GetVideoStatus_GpioFluctuatedForShortTime_Success) {
  std::thread gpio_updater = std::thread([this]() {
    TurnOff();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    TurnOn();
  });
  EXPECT_TRUE(video_status_->GetVideoStatus()) << "Status should be on";
  gpio_updater.join();
}

TEST_F(VideoStatusTest, GetVideoStatus_GpioKeepOffLongTime_False) {
  TurnOff();
  std::thread gpio_updater = std::thread([this]() {
    std::this_thread::sleep_for(std::chrono::milliseconds(600));
    TurnOn();
  });
  EXPECT_FALSE(video_status_->GetVideoStatus()) << "Status should be off";
  gpio_updater.join();
}

}  // namespace