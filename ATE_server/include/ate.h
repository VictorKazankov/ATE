#ifndef ATE_SERVER_ATE_H_
#define ATE_SERVER_ATE_H_

#include <chrono>
#include <memory>

#include <boost/asio/io_context.hpp>

#include "interaction/VDP/interaction.h"
#include "reporting/reporter.h"
#include "storage/storage.h"
#include "video_streaming/matching/matcher.h"
#include "video_streaming/streamer.h"

/**
 * Automated Test Environment class for communication with LVDS board
 **/
class ATE {
 private:
  std::unique_ptr<storage::Storage> storage_;
  std::unique_ptr<interaction::Interaction> interaction_;
  std::unique_ptr<reporter::Reporter> reporter_;
  detector::Matcher matcher_;

 public:
  ATE(boost::asio::io_context& io_context);
  ~ATE();
  /*
   * @brief Waits until the objectOrName object is accessible (i.e., it exists and is visible and enabled).
   * with values of x, y, width, height zero
   * @params name object or name of component
   * @params timeout_msec timeout for detection of object
   * @return returns a rectangle with specified coordinates of the object if successful, otherwise empty rectangle.
   */
  cv::Rect waitForObject(const std::string& name, const std::chrono::milliseconds& timeout);

  /*
   * @brief This function tells test to sleep for the specified nominal number of milliseconds.
   * @params timeout_msec milliseconds timeout
   */
  void Snooze(const std::chrono::milliseconds& duration);
  void Close();
};

#endif  // ATE_SERVER_ATE_H_
