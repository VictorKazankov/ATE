#include "utils/gpio_reader.h"

#include <cerrno>
#include <exception>
#include <fstream>

#include "logger/logger.h"

namespace utils {

GpioReader::GpioReader(const std::string& path) : gpio_path_(path) {
  std::ifstream fs_value(GetValuePath(), std::ifstream::in);

  if (!gpio_path_.empty() && fs_value.is_open()) {
    is_gpio_exist_ = true;
  } else {
    logger::info("[gpio reader] GPIO file system not exist");
    is_gpio_exist_ = false;
  }
}

GpioReader::~GpioReader() {}

bool GpioReader::CheckGpioAvailable() const { return is_gpio_exist_; }

bool GpioReader::GetStatus() const {
  std::string value;
  if (!is_gpio_exist_) {
    return false;
  } else {
    std::ifstream fs_value(GetValuePath(), std::ifstream::in);
    if (!std::getline(fs_value, value)) {
      logger::error("[gpio reader] reading file {} error {}", GetValuePath(), strerror(errno));
      return false;
    }
    if (value != "0" && value != "1") {
      logger::error("[gpio reader] line processing {} unknown value error", value);
      return false;
    }
  }
  return (value == "1");
}

}  // namespace utils
