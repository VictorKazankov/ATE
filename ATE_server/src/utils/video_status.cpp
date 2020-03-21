#include <thread>

#include "utils/video_status.h"

#include "utils/gpio_reader.h"

namespace utils {

GpioVideoStatus::GpioVideoStatus(const std::string& gpio_path) : video_gpio_(gpio_path) {}

GpioVideoStatus::~GpioVideoStatus() {}

bool GpioVideoStatus::GetVideoStatus() {
  // If gpio not available, ignore it
  if (!video_gpio_.CheckGpioAvailable()) {
    return true;
  }

  // Keep checking the gpio value for period of time
  const auto sample_max = 10;
  const auto sleep_50_ms = 50;
  for (auto sample_cnt = 0; sample_cnt < sample_max; ++sample_cnt) {
    if (video_gpio_.GetStatus()) {
      return true;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(sleep_50_ms));
  }

  return false;
}

}  // namespace utils
