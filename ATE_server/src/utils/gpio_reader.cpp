#include "utils/gpio_reader.h"

#include <cerrno>
#include <exception>
#include <fstream>

#include "logger/logger.h"

namespace utils {

GpioReader::GpioReader(const std::string& path) : gpio_path_(path) {}
GpioReader::~GpioReader() {}

bool GpioReader::GetStatus() const {
  auto status = false;
  if (gpio_path_.empty()) {
    return true;
  } else {
    std::string value_path = gpio_path_ + "/value";
    std::ifstream fs_value(value_path, std::ifstream::in);
    if (!fs_value.is_open()) {
      logger::error("[gpio reader] open file {} error {}", value_path, strerror(errno));
      return false;
    }

    std::string procesing_line;
    if (!std::getline(fs_value, procesing_line)) {
      logger::error("[gpio reader] reading file {} error {}", value_path, strerror(errno));
      return false;
    }

    if(procesing_line != "0" && procesing_line != "1") {
      logger::error("[gpio reader] line processing {} unknown value error", procesing_line);
      return false;
    }
    status = (procesing_line == "1");
  }

  return status;
}

}  // namespace utils
