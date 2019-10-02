#ifndef ATE_SERVER_ATE_H_
#define ATE_SERVER_ATE_H_

#include <chrono>
#include <memory>

#include <db_manager/db_manager.h>

#include <boost/asio/io_context.hpp>
#include "interaction/interaction.h"
#include "video_streaming/matching/matcher.h"
#include "video_streaming/streamer.h"

/**
 * Automated Test Environment class for communication with LVDS board
 **/
class ATE {
 private:
  std::unique_ptr<interaction::Interaction> interaction_;
  detector::Matcher matcher_;
  std::unique_ptr<db_manager::IDbManager> storage_;

 public:
  explicit ATE(boost::asio::io_context& io_context);
  ~ATE();

  /**
   * @brief performs a touch tap at the position specified by point
   */
  void TapObject(const cv::Point& point);

  /**
   * @brief performs a touch-based drag operation
   * @param object_or_name object or name of the component
   * @param start_point start coordinates
   * @param delta_point delta coordinates
   */
  void TouchAndDrag(const std::string& object_or_name, const cv::Point& start_point, const cv::Point& delta_point);

  /**
   * @brief Waits until the object is accessible (i.e., it exists and is visible and enabled)
   * @param object_or_name object or name of the component
   * @param timeout timeout for detection of the object
   * @return rectangle with specified coordinates of the object if successful, otherwise empty rectangle
   */
  cv::Rect WaitForObject(const std::string& object_or_name, const std::chrono::milliseconds& timeout);

  /**
   * @brief The function changes resolution for video stream
   * @param x - a new high value
   * @param y - a new width value
   */
  void ChangeResolution(int x, int y);
};

#endif  // ATE_SERVER_ATE_H_
