#ifndef API_AGGREGATOR_H_
#define API_AGGREGATOR_H_

#include "squish/application_context.h"

#include <memory>

#include "utils/geometry_types.h"
#include "utils/squish_types.h"

#include "ate_api.h"
#include "interaction.h"
#include "squish/squish_API.h"
#include "squish/squish_types.h"

namespace API {

/**
 * @brief ApiAggregator class represents APIs which can be processed by ATE.
 *        It incapsulates into itself squish's and ATE's APIs
 **/
class ApiAggregator {
 public:
  ApiAggregator();

  /**
   * @brief This function causes to attach to the application called aut_name.
   * @param aut_name (ignored) the name of an application that has been registered with the squishserver as an
   *        attachable AUT
   * @return A handle to its application context
   **/
  squish::ApplicationContext& AttachToApplication(const std::string& aut_name);

  /**
   * @brief WaitForObject waits until the objectOrName object is accessible (i.e., it exists and is visible and
   *        enabled). The function waits for the time defined by the testSettings.waitForObjectTimeout property, that
   *        many milliseconds
   * @param object_or_name Desirable object or name
   * @returns The object if successful or raises an exception on failure.
   * @throw LookupError In case of the pattern is not detected on the screen or timeout has expired
   * @throw VideoStreamingError In case of the video stream is not available
   * @throw NoConnectionEstablished In case of no connection was established to server-side
   * @throw invalid_argument In case of the invalid arguments in request
   * @throw runtime_error In case of an internal error, parse error, invalid request, a method not found
   **/
  squish::Object WaitForObject(const std::string& object_or_name);
  squish::Object WaitForObject(const squish::Object& object_or_name);

  /**
   * @brief WaitForObject waits until the objectOrName object is accessible (i.e., it exists and is visible and
   *        enabled). The function waits for the time defined by the testSettings.waitForObjectTimeout property, that
   *        many milliseconds
   * @param wildcard Pattern for selection
   * @param timeout_msec Timeout in miliseconds
   * @returns The object if successful or raises an exception on failure.
   * @throw LookupError In case of the pattern is not detected on the screen or timeout has expired
   * @throw VideoStreamingError In case of the video stream is not available
   * @throw NoConnectionEstablished In case of no connection was established to server-side
   * @throw invalid_argument In case of the invalid arguments in request
   * @throw runtime_error In case of an internal error, parse error, invalid request, a method not found
   **/
  squish::Object WaitForObject(const squish::Wildcard& wildcard);
  squish::Object WaitForObject(const squish::Wildcard& wildcard, int timeout_msec);

  /**
   * @brief WaitForObject waits until the objectOrName object is accessible (i.e., it exists and is visible and
   *        enabled). The function waits for the time defined by the optional timeoutMSec parameter is used, that many
   *        milliseconds. This function is useful if you want to synchronize your script execution.
   * @param object_or_name Desirable object or name
   * @param timeout_msec Timeout in miliseconds
   * @returns The object if successful or raises an exception on failure.
   * @throw LookupError In case of the pattern is not detected on the screen or timeout has expired
   * @throw VideoStreamingError In case of the video stream is not available
   * @throw NoConnectionEstablished In case of no connection was established to server-side
   * @throw invalid_argument In case of the invalid arguments in request
   * @throw runtime_error In case of an internal error, parse error, invalid request, a method not foun
   **/
  squish::Object WaitForObject(const std::string& object_or_name, int timeout_msec);
  squish::Object WaitForObject(const squish::Object& object_or_name, int timeout_msec);

  /**
   * @brief TapObject performs a touch tap at the position specified by screenPoint.
   * @param screen_point Position are in screen global coordinates
   * @param modifierState (optional) Specify keyboard modifiers that are held down during the click
   * @param button (optional) Specify mouse button that held down during the click
   * @throw NoConnectionEstablished In case of no connection was established to server-side
   * @throw invalid_argument In case of the invalid arguments in request
   * @throw runtime_error In case of an internal error, parse error, invalid request, a method not found
   **/
  void TapObject(const common::Point& screen_point, common::squish::ModifierState modifier_state,
                 common::squish::MouseButton button);

  /**
   * @brief TapObject performs a touch tap at the center of the rectangle specified by screenRectangle.
   * @param screen_rectangle Position are in screen global coordinates
   * @param modifierState (optional) Specify keyboard modifiers that are held down during the click
   * @param button (optional) Specify mouse button that held down during the click
   * @throw NoConnectionEstablished In case of no connection was established to server-side
   * @throw invalid_argument In case of the invalid arguments in request
   * @throw runtime_error In case of an internal error, parse error, invalid request, a method not found
   **/
  void TapObject(const common::Rect& screen_rectangle, common::squish::ModifierState modifier_state,
                 common::squish::MouseButton button);
  void TapObject(const squish::Object& screen_rectangle, common::squish::ModifierState modifier_state,
                 common::squish::MouseButton button);

  /**
   * @brief This function is pressing on the specified object a pointed amount of milliseconds, or 2 seconds if timeout
   *        not specified. The x and y coordinates are optional. If they are not specified the tap is made in the center
   *        of the widget. On the other hand, if the additional parameters are given, the tap is made at position
   *        x and y (in the object coordinates).
   * @param screen_rectangle Position are in screen global coordinates
   * @param x Relative coordinates in the object, optional
   * @param y Relative coordinates in the object, optional
   * @param timeout_msec Timeout in miliseconds between press event and release event, optional
   * @throw InvalidDurationLongPress In case if the press is longer than 60 seconds.
   * @throw NoConnectionEstablished In case of no connection was established to server-side
   * @throw invalid_argument In case of the invalid arguments in request
   * @throw runtime_error In case of an internal error, parse error, invalid request, a method not found
   **/
  void LongPress(const squish::Object& screen_rectangle, int timeout_msec);
  void LongPress(const squish::Object& screen_rectangle, int x, int y, int timeout_msec);

  /**
   * @brief TouchAndDrag performs a touch-based drag operation. It initiates a touch drag of the specified objectOrName
   *        widget starting at position x, y(in the objectOrName widget's coordinates). The objectOrName widget is
   *        dragged by dx pixels horizontally and by dy pixels vertically.
   * @param object_or_name Desirable object or name
   * @param x Start x coordinate of drag event
   * @param y Start y coordinate of drag event
   * @param dx Dragged by pixels horizontally
   * @param dy Dragged by pixels vertically
   * @param modifier_state Modifier state [NONE, ALT, CONTROL, SHIFT]
   * @throw NoConnectionEstablished In case of no connection was established to server-side
   * @throw invalid_argument In case of the invalid arguments in request
   * @throw runtime_error In case of an internal error, parse error, invalid request, a method not found
   **/
  void TouchAndDrag(const squish::Object& object_or_name, int x, int y, int dx, int dy,
                    common::squish::ModifierState modifier_state);
  void TouchAndDrag(const std::string& object_or_name, int x, int y, int dx, int dy,
                    common::squish::ModifierState modifier_state);

  /**
   * @brief This function performs a press and hold operation specified by a screen_point. Position is in screen global
   *        coordinates. Press and hold operation doesn't have a timeout for execution and can be interrupted by
   *        pressRelease() API only.
   *
   * Any subsequent call of any other touch based operation prior calling pressRelease() will cause undefined behaviour.
   *
   * @param screen_point Point in absolute coordinates where to perform the press operation
   * @throw NoConnectionEstablished In case of no connection was established to server-side
   * @throw invalid_argument In case of the invalid arguments in request
   * @throw runtime_error In case of an internal error, parse error, invalid request, a method not found
   **/
  void PressAndHold(const common::Point& screen_point);

  /**
   * @brief This function performs a press and hold operation specified by the center of a screen_rectangle. Position
   *        is in screen global coordinates. Press and hold operation doesn't have a timeout for execution and can be
   *        interrupted by pressRelease() API only.
   *
   * Any subsequent call of any other touch based operation prior calling pressRelease() will cause undefined behaviour.
   *
   * @param screen_rectangle Rectangle in absolute coordinates in the center of which to perform press operation
   * @throw NoConnectionEstablished In case of no connection was established to server-side
   * @throw invalid_argument In case of the invalid arguments in request
   * @throw runtime_error In case of an internal error, parse error, invalid request, a method not found
   **/
  void PressAndHold(const common::Rect& screen_rectangle);

  /**
   * @brief This function performs a press and hold operation specified by the center of the object obtained by
   *        waitForObject(). Press and hold operation doesn't have a timeout for execution and can be interrupted by
   *        pressRelease() API only.
   *
   * Any subsequent call of any other touch based operation prior calling pressRelease() will cause undefined behaviour.
   *
   * @param object Object obtained by waitForObject() in the center of which to perform press operation
   * @throw NoConnectionEstablished In case of no connection was established to server-side
   * @throw invalid_argument In case of the invalid arguments in request
   * @throw runtime_error In case of an internal error, parse error, invalid request, a method not found
   **/
  void PressAndHold(const squish::Object& object);

  /**
   * @brief This function performs a release operation to interrupt running pressAndHold() API at the position specified
   *        by screen_point in screen global coordinates.
   *
   * Passing argument other than in preceding pressAndHold() call will cause undefined behaviour.
   *
   * @param screen_point Point in absolute coordinates where to perform the release operation
   * @throw NoConnectionEstablished In case of no connection was established to server-side
   * @throw invalid_argument In case of the invalid arguments in request
   * @throw runtime_error In case of an internal error, parse error, invalid request, a method not found
   **/
  void PressRelease(const common::Point& screen_point);

  /**
   * @brief This function performs a release operation to interrupt running pressAndHold() API at the position specified
   *        by the center of the screen_rectangle in screen global coordinates.
   *
   * Passing argument other than in preceding pressAndHold() call will cause undefined behaviour.
   *
   * @param screen_rectangle Rectangle in absolute coordinates in the center of which to perform release operation
   * @throw NoConnectionEstablished In case of no connection was established to server-side
   * @throw invalid_argument In case of the invalid arguments in request
   * @throw runtime_error In case of an internal error, parse error, invalid request, a method not found
   **/
  void PressRelease(const common::Rect& screen_rectangle);

  /**
   * @brief This function performs a release operation to interrupt running pressAndHold() API at the position specified
   *        by object provided as a result of waitFordObject() API.
   *
   * Passing argument other than in preceding pressAndHold() call will cause undefined behaviour.
   *
   * @param object Object obtained by waitForObject() in the center of which to perform release operation
   * @throw NoConnectionEstablished In case of no connection was established to server-side
   * @throw invalid_argument In case of the invalid arguments in request
   * @throw runtime_error In case of an internal error, parse error, invalid request, a method not found
   **/
  void PressRelease(const squish::Object& object);

  /**
   * @brief This function verifies the object with the symbolic
   *        or real (multi-property) name objectName exists on the screen.
   * @param object_name Name of the object for verifying existence on the screen
   * @return Returns a true value if the object with the symbolic
   *         or real (multi-property) name objectName exists otherwise false
   * @throw VideoStreamingError In case of the video stream is not available
   * @throw NoConnectionEstablished In case of no connection was established to server-side
   * @throw invalid_argument In case of invalid params in request
   * @throw runtime_error In case of internal error, parse error, invalid request, method not found
   **/
  bool Exists(const std::string& object_name);

  /**
   * @brief This API allows changing sync version and sync build version without restart ate server.
   * @param sync_version Sync version
   * @param sync_build_version Sync build version
   * @throw InvalidSyncVersion In case of the sync_version is incorrect
   * @throw InvalidSyncBuildVersion In case of the sync_build_version is incorrect
   * @throw NoConnectionEstablished In case of no connection was established to server-side
   * @throw invalid_argument In case of the invalid arguments in the request
   * @throw runtime_error In case of an internal error, parse error, invalid request, a method not found
   */
  void ChangeSyncIconDB(const std::string& sync_version, const std::string& sync_build_version);

  /**
   * @brief ChangeSyncMode provides changing collection mode API for active sync
   * @param collection_mode Collection mode
   * @throw NoConnectionEstablished In case of no connection was established to server-side
   * @throw InvalidSyncCollectionMode In case of the collection_mode is incorrect
   * @throw invalid_argument In case of the invalid arguments in request
   * @throw runtime_error In case of an internal error, parse error, invalid request, a method not found
   */
  void ChangeSyncMode(common::squish::CollectionMode collection_mode);

  /**
   * @brief This API allows to take a screenshot of current screen and store it on the LVDS board
   * @param filename Name of the file (must contain .png extension for successfully saving)
   * @param location Location for saving of screenshot
   * @return True in case of screenshot saved
   * @throw VideoStreamNotFound In case of the video stream is not available
   * @throw EmptyScreenshotFileName In case of filename is empty"
   * @throw WrongScreenshotExtension In case of filename extansion is not 'png'
   * @throw PermissionDenied In case of server does not have permission to make directory
   * @throw ImageAssemblingFailed In case of server can't save the screenshot (only in case of cv::imwrite failed)
   * @throw InternalError In case of file system errors, bad alloc
   * @throw NoConnectionEstablished In case of no connection was established to server-side
   */
  bool GetScreenshot(const std::string& filename, const std::string& location);

  /**
   * @brief GetText returns the text by specified coordinates
   * @param x1 X axis of the topleft coordinate
   * @param y1 Y axis of the topleft coordinate
   * @param x2 X axis of the bottom-right coordinate
   * @param y2 Y axis of the bottom-right coordinate
   * @return Text
   * @throw NoConnectionEstablished In case of no connection was established to server-side
   * @throw VideoStreamNotFound In case of the video stream is not available
   * @throw invalid_argument In case of the invalid arguments in request
   * @throw runtime_error In case of an internal error, parse error, invalid request, a method not found
   */
  std::string GetText(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

  /**
   * @brief Gets object list by selecting a pattern
   * @param wildcard Pattern for selection
   * @param object_name Name of Object
   * @return List of objects
   * @throw NoConnectionEstablished In case of no connection was established to server-side
   * @throw invalid_argument In case of the invalid arguments in request
   * @throw runtime_error In case of an internal error, parse error, invalid request, a method not found
   */
  std::vector<squish::Object> GetObjectsDataByPattern(const squish::Wildcard& wildcard);
  std::vector<squish::Object> GetObjectsDataByPattern(const std::string& object_name);

  /**
   * @brief Gets the difference between the two images as a percentage
   * @param icon_path_second Path to the second image
   * @param icon_path_first Path to the first image
   * @param top_left_coordinate The top left coordinate of the comparison area
   * @param bottom_right_coordinate The bottom right coordinate of the comparison area
   * @return Percentage of discrepancy
   * @throw InvalidRectangleCoordinates In case top-left and bottom-right coordinates are mixed up or
   *                                    produced rectangle has zero height/width or is out of frame boundaries
   * @throw ComparingImageIncorrectSize In case incorrect size of image
   * @throw ComparingImageNotExist In case comparing images doesn't exist
   * @throw EmptyScreenshotFileName In case comparing image file path is empty
   * @throw PermissionDenied In case error read permissions
   * @throw NoConnectionEstablished In case of no connection was established to server-side
   * @throw invalid_argument In case of the invalid arguments in request
   * @throw runtime_error In case of an internal error, parse error, invalid request, a method not found
   * @throw UnsupportedFileType In case of incorrect image file type
   */
  int GetImagesDiscrepancy(const std::string& icon_path_second, const std::string& icon_path_first,
                           const common::Point& top_left_coordinate, const common::Point& bottom_right_coordinate);

  /** @brief CaptureFrames saves frames in specified path with desired area and returns an array of stored frame names
   * @param interval Interval in milliseconds
   * @param duration Duration in milleseconds (limit 5 sec)
   * @param top_left Top left point
   * @param bottom_right Bottom right point
   * @param path Path where frames will be stored
   * @return List of frame names
   * @throw VideoStreamNotFound In case of the video stream is not available
   * @throw PermissionDenied In case of server does not have permission to make directory
   * @throw InvalidDurationLongPress In case if the duration longer than 5 sec // TODO rename
   * @throw ImageAssemblingFailed In case of server can't save the screenshot (only in case of cv::imwrite failed)
   * @throw InternalError In case of file system errors, bad alloc
   * @throw NoConnectionEstablished In case of no connection was established to server-side
   */
  std::vector<std::string> CaptureFrames(int interval, int duration, const common::Point& top_left,
                                         const common::Point& bottom_right, const std::string& path);

 private:
  /**
   * @brief The function gets next correlation id for RPC
   * @return correlation id for RPC
   **/
  uint64_t GetCorrelationId();

  /**
   * @brief The function throws "NoConnectionEstablished" exception in case no connection was established (wasn't
   *        performed AttachToApplication)
   * @throw NoConnectionEstablished In case of no connection was established to server-side
   **/
  void ThrowExceptionIfNoConnectionEstablished() const;

 private:
  std::shared_ptr<interaction::Interaction> ate_interaction_;
  uint64_t correlation_id_ = 1;

  AteApi ate_api_;
  SquishApi squish_api_;
};
}  // namespace API

#endif  // API_AGGREGATOR_H_
