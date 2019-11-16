#ifndef ATE_SERVER_UTILS_GPIO_READER_H_
#define ATE_SERVER_UTILS_GPIO_READER_H_

#include <string>

namespace utils {

/**
 * @brief Class for READING GPIO status in Linux system.
 * GPIO file system interface is provided by Linux.
 * Detail can be found at https://www.kernel.org/doc/Documentation/gpio/sysfs.txt
 * GPIO reader is created with a path, this directory structure should following the gpio file system.
 * GPIO signals have paths like /sys/class/gpio/gpioN/ and have following read/write attributes:
 * direction, value, edge and active_low.
 **/
class GpioReader {
 public:
  explicit GpioReader(const std::string& path);

  /**
   * @brief read the the status of value attribute
   * @return if gpio is disable or not exported, return true
   * if gpio is configure and exported return gpio value (if value is 0 return false otherwise return false)
   */
  bool GetStatus() const;
  ~GpioReader();

 private:
  std::string gpio_path_;
};

}  // namespace utils

#endif
