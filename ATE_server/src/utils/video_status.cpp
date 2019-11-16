#include "utils/video_status.h"

#include "utils/env_config.h"
#include "utils/gpio_reader.h"

namespace utils {

GpioVideoStatus::GpioVideoStatus() : video_gpio_(GetEnvSettings(EnvSettings::kGpioVideoStatusPath)) {}

bool GpioVideoStatus::GetVideoStatus() {
  bool status = false;
  if (video_gpio_.GetStatus(status) && !status) {
    return false;
  }
  return true;
}

}  // namespace utils