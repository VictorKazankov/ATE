#include "utils/video_status.h"

#include "utils/env_config.h"
#include "utils/gpio_reader.h"

namespace utils {

GpioVideoStatus::GpioVideoStatus() : video_gpio_(GetEnvSettings(EnvSettings::kGpioVideoStatusPath)) {}

GpioVideoStatus::~GpioVideoStatus() {}

/**
 * TODO: Refactor reading GPIO status in case LVDS board had a bug and change the status on and of at radom time.
 * The second question is, how it can help if LVDS give wrong value of GPIO.
 * Disable this feature temporary.
 */
bool GpioVideoStatus::GetVideoStatus() { return true; }

}  // namespace utils
