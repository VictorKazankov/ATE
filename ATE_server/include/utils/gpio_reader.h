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
   * @brief Read the the status of value attribute
   * @return Return gpio value on succeed (e.g. gpio supported and available)
   *         on failure return false
   */
  bool GetStatus() const;

  /**
   * @brief Check if the gpio file system is available to use
   * @return Return true if gpio path has been available for using
   *         otherwise return false
   */
  bool CheckGpioAvailable() const;

  ~GpioReader();

 private:
  std::string GetValuePath() const { return {gpio_path_ + "/value"}; };
  std::string gpio_path_;
  bool is_gpio_exist_;
};

}  // namespace utils

#endif
