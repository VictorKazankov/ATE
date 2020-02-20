#ifndef API_AGGREGATOR_H_
#define API_AGGREGATOR_H_

#include "squish/application_context.h"

#include <memory>

#include "utils/geometry_types.h"
#include "utils/squish_types.h"

#include "ate_api.h"
#include "ate_interaction.h"
#include "squish/squish_API.h"
#include "squish/squish_types.h"

namespace API {

/**
 * @brief ApiAggregator class represents APIs which can be processed by ATE.
 * It incapsulates into itself squish's and ATE's APIs
 **/
class ApiAggregator {
 public:
  ApiAggregator();

  /**
   * @brief This function causes to attach to the application called aut_name.
   * @param aut_name (ignored) the name of an application that has been registered with the squishserver as an
   *attachable AUT
   * @return A handle to its application context
   **/
  squish::ApplicationContext& AttachToApplication(const std::string& aut_name);

  /**
   * @brief WaitForObject waits until the objectOrName object is accessible (i.e., it exists and is visible and
   *enabled). The function waits for the time defined by the testSettings.waitForObjectTimeout property, that many
   *milliseconds
   * @param _objectOrName - desirable object or name
   * @returns the object if successful or raises a (catchable) LookupError, VideoStreamingError exception on failure,
   *i.e., if the function times out, the video stream is not found.
   **/
  squish::Object WaitForObject(const std::string& object_or_name);
  squish::Object WaitForObject(const squish::Object& object_or_name);

  /**
   * @brief WaitForObject waits until the objectOrName object is accessible (i.e., it exists and is visible and
   *enabled). The function waits for the time defined by the optional timeoutMSec parameter is used, that many
   *milliseconds. This function is useful if you want to synchronize your script execution.
   * @param _objectOrName - desirable object or name
   * @param timeoutMSec - timeout in miliseconds
   * @returns the object if successful or raises a (catchable) LookupError, VideoStreamingError exception on failure,
   *i.e., if the function times out, the video stream is not found.
   **/
  squish::Object WaitForObject(const std::string& object_or_name, int timeout_msec);
  squish::Object WaitForObject(const squish::Object& object_or_name, int timeout_msec);

  /**
   * @brief TapObject performs a touch tap at the position specified by screenPoint.
   * @param screen_point - position are in screen global coordinates
   * @param The optional modifierState specify keyboard modifiers that are held down during the click
   * @param The optional button specify mouse button that held down during the click
   **/

  void TapObject(const common::Point& screen_point, common::squish::ModifierState modifier_state,
                 common::squish::MouseButton button);

  /**
   * @brief TapObject performs a touch tap at the center of the rectangle specified by screenRectangle.
   * @param screen_rectangle - position are in screen global coordinates
   * @param The optional modifierState specify keyboard modifiers that are held down during the click
   * @param The optional button specify mouse button that held down during the click
   **/
  void TapObject(const common::Rect& screen_rectangle, common::squish::ModifierState modifier_state,
                 common::squish::MouseButton button);
  void TapObject(const squish::Object& screen_rectangle, common::squish::ModifierState modifier_state,
                 common::squish::MouseButton button);

  /**
   * @brief This function is pressing on the specified object a pointed amount of milliseconds, or 2 seconds if timeout
   * not specified. The x and y coordinates are optional. If they are not specified the tap is made in the center of the
   * widget. On the other hand, if the additional parameters are given, the tap is made at position x and y (in the
   * object coordinates).
   * @param screen_rectangle - position are in screen global coordinates
   * @param x - relative coordinates in the object, optional
   * @param y - relative coordinates in the object, optional
   * @param timeout_msec - timeout in miliseconds between press event and release event, optional
   * @exception Throws InvalidDurationLongPress in case if the press is longer than 60 seconds.
   **/
  void LongPress(const squish::Object& screen_rectangle, int timeout_msec);
  void LongPress(const squish::Object& screen_rectangle, int x, int y, int timeout_msec);

  /**
   * @brief TouchAndDrag performs a touch-based drag operation. It initiates a touch drag of the specified objectOrName
   *  widget starting at position x, y(in the objectOrName widget's coordinates). The objectOrName widge is dragged by
   *  dx pixels horizontally and by dy pixels vertically.
   * @param object_or_name - desirable object or name
   * @param x - start x coordinate of drag event
   * @param y - start y coordinate of drag event
   * @param dx - dragged by pixels horizontally
   * @param dy - dragged by pixels vertically
   * @param modifier_state - modifier state [NONE, ALT, CONTROL, SHIFT]
   **/
  void TouchAndDrag(const squish::Object& object_or_name, int x, int y, int dx, int dy,
                    common::squish::ModifierState modifier_state);
  void TouchAndDrag(const std::string& object_or_name, int x, int y, int dx, int dy,
                    common::squish::ModifierState modifier_state);

  /**
   * @brief This function performs a press and hold operation specified by a screen_point. Position is in screen global
   * coordinates. Press and hold operation doesn't have a timeout for execution and can be interrupted by pressRelease()
   * API only.
   *
   * Any subsequent call of any other touch based operation prior calling pressRelease() will cause undefined behaviour.
   *
   * @param screen_point Point in absolute coordinates where to perform the press operation
   **/
  void PressAndHold(const common::Point& screen_point);

  /**
   * @brief This function performs a press and hold operation specified by the center of a screen_rectangle. Position
   * is in screen global coordinates. Press and hold operation doesn't have a timeout for execution and can be
   * interrupted by pressRelease() API only.
   *
   * Any subsequent call of any other touch based operation prior calling pressRelease() will cause undefined behaviour.
   *
   * @param screen_rectangle Rectangle in absolute coordinates in the center of which to perform press operation
   **/
  void PressAndHold(const common::Rect& screen_rectangle);

  /**
   * @brief This function performs a press and hold operation specified by the center of the object obtained by
   * waitForObject(). Press and hold operation doesn't have a timeout for execution and can be interrupted by
   * pressRelease() API only.
   *
   * Any subsequent call of any other touch based operation prior calling pressRelease() will cause undefined behaviour.
   *
   * @param object Object obtained by waitForObject() in the center of which to perform press operation
   **/
  void PressAndHold(const squish::Object& object);

  /**
   * @brief This function performs a release operation to interrupt running pressAndHold() API at the position specified
   * by screen_point in screen global coordinates.
   *
   * Passing argument other than in preceding pressAndHold() call will cause undefined behaviour.
   *
   * @param screen_point Point in absolute coordinates where to perform the release operation
   **/
  void PressRelease(const common::Point& screen_point);

  /**
   * @brief This function performs a release operation to interrupt running pressAndHold() API at the position specified
   * by the center of the screen_rectangle in screen global coordinates.
   *
   * Passing argument other than in preceding pressAndHold() call will cause undefined behaviour.
   *
   * @param screen_rectangle Rectangle in absolute coordinates in the center of which to perform release operation
   **/
  void PressRelease(const common::Rect& screen_rectangle);

  /**
   * @brief This function performs a release operation to interrupt running pressAndHold() API at the position specified
   * by object provided as a result of waitFordObject() API.
   *
   * Passing argument other than in preceding pressAndHold() call will cause undefined behaviour.
   *
   * @param object Object obtained by waitForObject() in the center of which to perform release operation
   **/
  void PressRelease(const squish::Object& object);

  /**
   * @brief This function verifies the object with the symbolic
   * or real (multi-property) name objectName exists on the screen.
   * @param object_name - name of the object for verifying existence on the screen
   * @return returns a true value if the object with the symbolic
   * or real (multi-property) name objectName exists otherwise false
   **/
  bool Exists(const std::string& object_name);

  /**
   * @brief This API allows changing sync version and sync build version without restart ate server.
   * @param sync_version Sync version
   * @param sync_build_version Sync build version
   */
  void ChangeSyncIconDB(const std::string& sync_version, const std::string& sync_build_version);

  /**
   * @brief ChangeSyncMode provides changing collection mode API for active sync
   * @param collection_mode Collection mode
   */
  void ChangeSyncMode(common::squish::CollectionMode collection_mode);

  /**
   * @brief This API allows to take a screenshot of current screen and store it on the LVDS board
   * @param filename - name of the file (must contain .png extension for successfully saving)
   * @param location - location for saving of screenshot
   * @return true in case of screenshot saved
   * @throw VideoStreamNotFound - in case of the video stream is not available
   * @throw EmptyScreenshotFileName - in case of filename is empty"
   * @throw WrongScreenshotExtension - in case of filename extansion is not 'png'
   * @throw PermissionDenied - in case of server does not have permission to make directory
   * @throw ImageAssemblingFailed - in case of server can't save the screenshot (only in case of cv::imwrite failed)
   * @throw InternalError - in case of file system errors, bad alloc
   */
  bool GetScreenshot(const std::string& filename, const std::string& location);

  /**
   * @brief GetText returns the text by specified coordinates
   * @param x1 x axis of the topleft coordinate
   * @param y1 y axis of the topleft coordinate
   * @param x2 x axis of the bottom-right coordinate
   * @param y2 y axis of the bottom-right coordinate
   */
  std::string GetText(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

  /**
   * @brief Gets object list by object name
   * @param object_name Name of object
   * @return List of objects
   */
  std::vector<squish::Object> GetObjectsDataByPattern(const std::string& object_name);

  /**
   * @brief Gets object list by selecting a pattern
   * @param object_pattern Pattern for selection
   * @return List of objects
   */
  std::vector<squish::Object> GetObjectsDataByPattern(const squish::Object& object_pattern);

 private:
  /**
   * @brief The function gets next correlation id for RPC
   * @return correlation id for RPC
   **/
  uint64_t GetCorrelationId();

  /**
   * @brief The function throws "NoConnectionEstablished" exception in case no connection was established (wasn't
   * performed AttachToApplication)
   * @throw NoConnectionEstablished - in case of no connection was established to server-side
   **/
  void ThrowExceptionIfNoConnectionEstablished() const;

 private:
  std::shared_ptr<interaction::ATEInteraction> ate_interaction_;
  uint64_t correlation_id_ = 1;

  AteApi ate_api_;
  SquishApi squish_api_;
};
}  // namespace API

#endif  // API_AGGREGATOR_H_
