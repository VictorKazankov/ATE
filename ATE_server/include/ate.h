#ifndef ATE_SERVER_ATE_H_
#define ATE_SERVER_ATE_H_

#include <chrono>
#include <memory>

#include "adapters/db_manager_adapter.h"
#include "interaction/interaction.h"
#include "utils/object_data_type.h"
#include "video_streaming/matching/matcher.h"
#include "video_streaming/streamer.h"

/**
 * Automated Test Environment class for communication with LVDS board
 **/
class ATE {
 public:
  explicit ATE();
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
   * @brief Waits for specified timeout until Object identified by object_or_name becomes
   *        detectible on the screen and returns the detection area rectange of this Object.
   *        If image with name object_or_name exists in Truth DB for currently selected DB
   *        config (sync_version, build_version, mode) then Object is detected as image.
   *        Otherwise text is detected, which exactly matches object_or_name string.
   * @param object_or_name Name of the image in Truth DB or text to wait for
   * @param timeout Timeout in ms to wait. Object will be tried to detect at least once even if
   *                detection time overruns given timeout
   * @return Detection area rectangle and zero error code on success. If timeout runs out,
   *         then zero rectangle and AteError::kPatternNotFound error code. On other failure,
   *         zero rectangle and appropriate non-zero error code
   */
  std::pair<cv::Rect, std::error_code> WaitForObject(const std::string& object_or_name,
                                                     const std::chrono::milliseconds& timeout,
                                                     const cv::Rect& search_region);

  /**
   * @brief Waits for specified timeout until the image from Truth DB identified by
   *        object_data_identity will become detectible on the screen and returns
   *        the detection area rectangle of this image.
   *        object_data_identity may specify single image in Truth DB or multiple images if
   *        ObjectDataIdentity struct string fields contain wildcard characters (*, ?) or mode
   *        is set to kAny.
   *        In case multiple images match object_data_identity, the detection area of a first
   *        image detected on the screen will be returned.
   *        Thid method does not perform text detection. In case no image can be fetched from
   *        DB by object_data_identity, AteError::kPatternNotFound is returned
   * @param object_data_identity Identity of image in TruthDB or pattern for iamge search in Truth DB
   * @param timeout Timeout in ms to wait. All images that matched object_data_identity will be tried
   *                to detect at least once even if overall detection time overruns given timeout
   * @return Detection area rectangle of first found image and zero error code on success. If timeout
   *         runs out, then zero rectangle and Ate::ErrorCode::kPatternNotFound error code. On other
   *         failure, zero rectangle and appropriate non-zero error code
   */
  std::pair<cv::Rect, std::error_code> WaitForObject(const common::ObjectDataIdentity& object_data_identity,
                                                     const std::chrono::milliseconds& timeout,
                                                     const cv::Rect& search_region);

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

  /**
   * @brief The function extracts text from predefined area in accordance with coordinates
   * @param point of start of predefined rectangle
   * @param delta_point of end of predefined rectangle
   * @return string representation of detected text on predefined area
   */
  std::pair<std::string, std::error_code> GetText(const cv::Point& point, const cv::Point& delta_point);

  /**
   * @brief Gets a list of objects by select pattern
   * @param pattern Pattern for selecting
   * @param object_name Name of Object
   * @return List of data object
   */
  std::vector<common::ObjectData> GetObjectsDataByPattern(const common::ObjectDataIdentity& pattern);
  std::vector<common::ObjectData> GetObjectsDataByPattern(const std::string& object_name);

  /**
   * @brief Gets the difference between the two images as a percentage
   * @param icon_path_second Path to the second image
   * @param icon_path_first Path to the first image
   * @param top_left_coordinate Top left  coordinate
   * @param bottom_right_coordinate Bottom right coordinate
   * @return A discrepancy coefficient and error code of operation
   */
  std::pair<int, std::error_code> GetImagesDiscrepancy(const std::string& icon_path_second,
                                                       const std::string& icon_path_first,
                                                       const cv::Point& top_left_coordinate,
                                                       const cv::Point& bottom_right_coordinate) const;

  /**
   * @brief CaptureFrames saves captured frames per interval with specified duration
   * @param interval Interval in miliseconds
   * @param duratin Duration in miliseconds
   * @param area Desired area
   * @param path Path where frames stored
   * @param error If an error ocurred than error parameter will be filled, otherwise empty
   * @return List of frame names
   */
  std::vector<std::string> CaptureFrames(size_t interval, size_t duration, const cv::Rect& area,
                                         const std::string& path, std::error_code& error);

  /**
   * @brief Performs a search at video source frame for image pattern
   * @param object_or_name Image name from database
   * @param area Area for a search. If its empty, search performs by the whole frame
   * @return List of detected objects and an empty error code in case of success, otherwise - empty list and an error
   *         code
   */
  std::pair<std::vector<common::Rect>, std::error_code> FindAllImages(const std::string& object_or_name,
                                                                      const common::Rect& area);

 private:
  std::unique_ptr<interaction::Interaction> interaction_;
  detector::Matcher matcher_;
  adapter::DBManagerAdapter storage_;
};

#endif  // ATE_SERVER_ATE_H_
