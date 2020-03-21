#ifndef ATE_SERVER_UTILS_VIDEO_STATUS_H_
#define ATE_SERVER_UTILS_VIDEO_STATUS_H_

#include "utils/gpio_reader.h"

namespace utils {

/**
 * @brief The Interface for Video Status Readers
 */
class VideoStatus {
 public:
  virtual ~VideoStatus() = default;

  /**
   * @brief Function to get the status of video link
   * @return true if the status is available, false otherwise
   */
  virtual bool GetVideoStatus() = 0;
};

/**
 * @brief Class implement video status via reading Gpio
 */
class GpioVideoStatus : public VideoStatus {
 public:
  explicit GpioVideoStatus(const std::string& gpio_path);
  ~GpioVideoStatus() final;

  GpioVideoStatus(const GpioVideoStatus&) = delete;
  GpioVideoStatus& operator=(const GpioVideoStatus&) = delete;

  /**
   * @brief If the GPIO is configured and exported as system file.
   * we can read gpio value to provide the status of video link between board and SYNC
   * The function to check get the value from gpio reader and return the video status
   * @return true - if video link is on, false - off
   */
  bool GetVideoStatus() override;

 private:
  GpioReader video_gpio_;
};

}  // namespace utils
#endif
