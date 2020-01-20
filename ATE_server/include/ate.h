#ifndef ATE_SERVER_ATE_H_
#define ATE_SERVER_ATE_H_

#include <chrono>
#include <memory>

#include <boost/asio/io_context.hpp>
#include "adapters/db_manager_adapter.h"
#include "interaction/interaction.h"
#include "video_streaming/matching/matcher.h"
#include "video_streaming/streamer.h"

/**
 * Automated Test Environment class for communication with LVDS board
 **/
class ATE {
 public:
  explicit ATE(boost::asio::io_context& io_context);
  ~ATE();

  /**
   * @brief performs a touch tap at the position specified by point
   */
  void TapObject(const cv::Point& point);

  /**
   * @brief The LongPress function performs a touch and release event with delay in (timeout) milliseconds (2000
   * milliseconds by default)
   * @param x - distance from left screen edge to point horizontally that will be pressed in pixels.
   * @param y - distance from top screen edge to the point vertically that will be pressed in pixels.
   * @param timeout - delay between press end release on point
   * @return error_code - that holds error code number in case if timeout bigger than 60 seconds, otherwise empty error
   */
  std::error_code LongPress(uint16_t x, uint16_t y, const std::chrono::milliseconds& timeout);

  /**
   * @brief performs a touch-based drag operation
   * @param object_or_name object or name of the component
   * @param start_point start coordinates
   * @param delta_point delta coordinates
   */
  void TouchAndDrag(const std::string& object_or_name, const cv::Point& start_point, const cv::Point& delta_point);

  /**
   * @brief This function performs a press operation in a specified point and holds the press infinitely until the
   * release operation is called.
   *
   * If any other press based operation is followed before the release (TapObject, TouchAndDrag, LongPress or etc.)
   * the behaviour is undefined.
   *
   * @param point Point in absolute coordinates where to perform the press operation
   */
  void PressAndHold(const cv::Point& point);

  /**
   * @brief This function performs a release for a previosuly executed PressAndHold operation.
   *
   * Release must be performed in the same point as a preceding PressAndHold. The behaviour is undefined otherwise.
   *
   * @param point Point in absolute coordinates where to perform the release operation
   */
  void PressRelease(const cv::Point& point);

  /**
   * @brief Waits until the object is accessible (i.e., it exists and is visible and enabled)
   * @param object_or_name object or name of the component
   * @param timeout timeout for detection of the object
   * @return: a pair of possition and error code which indicate the result of function, rectangle with specified
   * coordinates of the object if successful, otherwise return error code and empty rectangle
   */
  std::pair<cv::Rect, std::error_code> WaitForObject(const std::string& object_or_name,
                                                     const std::chrono::milliseconds& timeout);

  /**
   * @brief The function changes resolution for video stream
   * @param x - a new high value
   * @param y - a new width value
   */
  void ChangeResolution(int x, int y);

  /**
   * @brief Change sync configuration
   * @param sync_version Sync version
   * @param sync_build_version Sync build version
   * @return Error code if incorrect  sync_version or sync_build_version, otherwise success
   */
  adapter::DBManagerError ChangeSyncVersion(const std::string& sync_version, const std::string& sync_build_version);

  /**
   * @brief Changes sync collection mode
   * @param collection_mode Collection mode
   * @return Error if incorrect collection_mode, otherwise success
   */
  adapter::DBManagerError ChangeSyncMode(const std::string& collection_mode);

  /**
   * @brief The function reloads storage
   * @return Empty error if successed, otherwise error
   */
  std::error_code ReloadStorageItems() noexcept;

  /**
   * @brief The function gets current screenshot
   * @param path location for saving a screenshot
   * @param filename name of a file for saving a screenshot
   * @return empty error if successed, otherwise error
   */
  std::error_code GetScreenshot(const std::string& path, const std::string& filename);

 private:
  std::unique_ptr<interaction::Interaction> interaction_;
  detector::Matcher matcher_;
  adapter::DBManagerAdapter storage_;
};

#endif  // ATE_SERVER_ATE_H_
