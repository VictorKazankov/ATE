#include "utils/video_status.h"

#include "utils/env_config.h"
#include "utils/gpio_reader.h"

namespace utils {

GpioVideoStatus::GpioVideoStatus() : video_gpio_(GetEnvSettings(EnvSettings::kGpioVideoStatusPath)) {}

GpioVideoStatus::~GpioVideoStatus() {}

bool GpioVideoStatus::GetVideoStatus() { return video_gpio_.GetStatus(); }

}  // namespace utils
