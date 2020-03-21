#include "utils/gpio_reader.h"

#include <cerrno>
#include <exception>
#include <fstream>

#include "logger/logger.h"

namespace utils {

GpioReader::GpioReader(const std::string& path) : gpio_path_(path) {}
GpioReader::~GpioReader() {}

bool GpioReader::CheckGpioAvailable() const {
  if (gpio_path_.empty()) {
    return false;
  } else {
    std::ifstream fs_value(GetValuePath(), std::ifstream::in);
    if (!fs_value.is_open()) {
      logger::error("[gpio reader] open file {} error {}", GetValuePath(), strerror(errno));
      return false;
    }

    std::string procesing_line;
    if (!std::getline(fs_value, procesing_line)) {
      logger::error("[gpio reader] reading file {} error {}", GetValuePath(), strerror(errno));
      return false;
    }

    if (procesing_line != "0" && procesing_line != "1") {
      logger::error("[gpio reader] line processing {} unknown value error", procesing_line);
      return false;
    }
    return true;
  }
}

bool GpioReader::GetStatus() const {
  if (!CheckGpioAvailable()) {
    return false;
  } else {
    std::ifstream fs_value(GetValuePath(), std::ifstream::in);
    std::string value;
    std::getline(fs_value, value);
    return (value == "1");
  }
}

}  // namespace utils
