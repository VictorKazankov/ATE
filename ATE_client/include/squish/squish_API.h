#ifndef SQUISH_API_H_
#define SQUISH_API_H_

#include <chrono>
#include <string>

#include "utils/geometry_types.h"
#include "utils/squish_types.h"

#include "ate_interaction.h"
#include "error_defines.h"
#include "squish/application_context.h"
#include "squish/squish_types.h"

namespace API {

/**
 * @brief SquishApi class represents squish-like APIs which can be processed by ATE.
 **/
class SquishApi {
 public:
  /**
   * @brief This function causes to attach to the application called aut_name.
   * @param ate_interaction structure provides the ability to communicate with ATE
   * @param aut_name (ignored) the name of an application that has been registered with the squishserver as an
   *attachable AUT
   * @return A handle to its application context
   **/
  squish::ApplicationContext& AttachToApplication(const std::shared_ptr<interaction::ATEInteraction>& ate_interaction,
                                                  const std::string& aut_name = "");

  /**
   * @brief WaitForObject waits until the objectOrName object is accessible (i.e., it exists and is visible and
   *enabled). The function waits for the time defined by the testSettings.waitForObjectTimeout property, that many
   *milliseconds
   * @param ate_interaction structure provides the ability to communicate with ATE
   * @param correlation_id - correlation id for RPC
   * @param object_or_name - desirable object or name
   * @returns the object if successful or raises a (catchable) LookupError, VideoStreamingError exception on failure,
   *i.e., if the function times out, the video stream is not found.
   **/
  squish::Object WaitForObject(const std::shared_ptr<interaction::ATEInteraction>& ate_interaction,
                               const uint64_t& correlation_id, const std::string& object_or_name) const;
  squish::Object WaitForObject(const std::shared_ptr<interaction::ATEInteraction>& ate_interaction,
                               const uint64_t& correlation_id, const squish::Object& object_or_name) const;

  /**
   * @brief WaitForObject waits until the objectOrName object is accessible (i.e., it exists and is visible and
   *enabled). The function waits for the time defined by the optional timeoutMSec parameter is used, that many
   *milliseconds. This function is useful if you want to synchronize your script execution.
   * @param ate_interaction structure provides the ability to communicate with ATE
   * @param correlation_id - correlation id for RPC
   * @param _objectOrName - desirable object or name
   * @param timeoutMSec - timeout in miliseconds
   * @returns the object if successful or raises a (catchable) LookupError, VideoStreamingError exception on failure,
   *i.e., if the function times out, the video stream is not found.
   **/
  squish::Object WaitForObject(const std::shared_ptr<interaction::ATEInteraction>& ate_interaction,
                               const uint64_t& correlation_id, const std::string& object_or_name,
                               int timeout_msec) const;
  squish::Object WaitForObject(const std::shared_ptr<interaction::ATEInteraction>& ate_interaction,
                               const uint64_t& correlation_id, const squish::Object& object_or_name,
                               int timeout_msec) const;

  /**
   * @brief TapObject performs a touch tap at the position specified by screenPoint.
   * @param ate_interaction structure provides the ability to communicate with ATE
   * @param correlation_id - correlation id for RPC
   * @param screen_point - position are in screen global coordinates
   * @param The optional modifierState specify keyboard modifiers that are held down during the click
   * @param The optional button specify mouse button that held down during the click
   **/

  void TapObject(const std::shared_ptr<interaction::ATEInteraction>& ate_interaction, const uint64_t& correlation_id,
                 const common::Point& screen_point,
                 common::squish::ModifierState modifier_state = common::squish::ModifierState::NONE,
                 common::squish::MouseButton button = common::squish::MouseButton::NONE) const;

  /**
   * @brief TapObject performs a touch tap at the center of the rectangle specified by screenRectangle.
   * @param ate_interaction structure provides the ability to communicate with ATE
   * @param correlation_id - correlation id for RPC
   * @param screen_rectangle - position are in screen global coordinates
   * @param The optional modifierState specify keyboard modifiers that are held down during the click
   * @param The optional button specify mouse button that held down during the click
   **/
  void TapObject(const std::shared_ptr<interaction::ATEInteraction>& ate_interaction, const uint64_t& correlation_id,
                 const common::Rect& screen_rectangle,
                 common::squish::ModifierState modifier_state = common::squish::ModifierState::NONE,
                 common::squish::MouseButton button = common::squish::MouseButton::NONE) const;
  void TapObject(const std::shared_ptr<interaction::ATEInteraction>& ate_interaction, const uint64_t& correlation_id,
                 const squish::Object& screen_rectangle,
                 common::squish::ModifierState modifier_state = common::squish::ModifierState::NONE,
                 common::squish::MouseButton button = common::squish::MouseButton::NONE) const;

  /**
   * @brief This function is pressing on the specified object a pointed amount of milliseconds, or 2 seconds if timeout
   * not specified. The x and y coordinates are optional. If they are not specified the tap is made in the center of the
   * widget. On the other hand, if the additional parameters are given, the tap is made at position x and y (in the
   * object coordinates).
   * @param ate_interaction structure provides the ability to communicate with ATE
   * @param correlation_id - correlation id for RPC
   * @param screen_rectangle - position are in screen global coordinates
   * @param x - relative coordinates in the object, optional
   * @param y - relative coordinates in the object, optional
   * @param timeout_msec - timeout in miliseconds between press event and release event, optional
   * @exception Throws InvalidDurationLongPress in case if the press is longer than 60 seconds.
   **/
  void LongPress(const std::shared_ptr<interaction::ATEInteraction>& ate_interaction, const uint64_t& correlation_id,
                 const squish::Object& screen_rectangle, int timeout_msec) const;
  void LongPress(const std::shared_ptr<interaction::ATEInteraction>& ate_interaction, const uint64_t& correlation_id,
                 const squish::Object& screen_rectangle, int x, int y, int timeout_msec) const;

  /**
   * @brief TouchAndDrag performs a touch-based drag operation. It initiates a touch drag of the specified objectOrName
   *  widget starting at position x, y(in the objectOrName widget's coordinates). The objectOrName widge is dragged by
   *  dx pixels horizontally and by dy pixels vertically.
   * @param ate_interaction structure provides the ability to communicate with ATE
   * @param correlation_id - correlation id for RPC
   * @param object_or_name - desirable object or name
   * @param x - start x coordinate of drag event
   * @param y - start y coordinate of drag event
   * @param dx - dragged by pixels horizontally
   * @param dy - dragged by pixels vertically
   * @param modifier_state - modifier state [NONE, ALT, CONTROL, SHIFT]
   **/
  void TouchAndDrag(const std::shared_ptr<interaction::ATEInteraction>& ate_interaction, const uint64_t& correlation_id,
                    const squish::Object& object_or_name, int x, int y, int dx, int dy,
                    common::squish::ModifierState modifier_state) const;
  void TouchAndDrag(const std::shared_ptr<interaction::ATEInteraction>& ate_interaction, const uint64_t& correlation_id,
                    const std::string& object_or_name, int x, int y, int dx, int dy,
                    common::squish::ModifierState modifier_state) const;

  /**
   * @brief This function performs a press and hold operation specified by a screen_point. Position is in screen global
   * coordinates. Press and hold operation doesn't have a timeout for execution and can be interrupted by pressRelease()
   * API only.
   *
   * Any subsequent call of any other touch based operation prior calling pressRelease() will cause undefined behaviour.
   *
   * @param ate_interaction structure provides the ability to communicate with ATE
   * @param correlation_id - correlation id for RPC
   * @param screen_point Point in absolute coordinates where to perform the press operation
   **/
  void PressAndHold(const std::shared_ptr<interaction::ATEInteraction>& ate_interaction, const uint64_t& correlation_id,
                    const common::Point& screen_point) const;

  /**
   * @brief This function performs a press and hold operation specified by the center of a screen_rectangle. Position
   * is in screen global coordinates. Press and hold operation doesn't have a timeout for execution and can be
   * interrupted by pressRelease() API only.
   *
   * Any subsequent call of any other touch based operation prior calling pressRelease() will cause undefined behaviour.
   *
   * @param ate_interaction structure provides the ability to communicate with ATE
   * @param correlation_id - correlation id for RPC
   * @param screen_rectangle Rectangle in absolute coordinates in the center of which to perform press operation
   **/
  void PressAndHold(const std::shared_ptr<interaction::ATEInteraction>& ate_interaction, const uint64_t& correlation_id,
                    const common::Rect& screen_rectangle) const;

  /**
   * @brief This function performs a press and hold operation specified by the center of the object obtained by
   * waitForObject(). Press and hold operation doesn't have a timeout for execution and can be interrupted by
   * pressRelease() API only.
   *
   * Any subsequent call of any other touch based operation prior calling pressRelease() will cause undefined behaviour.
   *
   * @param ate_interaction structure provides the ability to communicate with ATE
   * @param correlation_id - correlation id for RPC
   * @param object Object obtained by waitForObject() in the center of which to perform press operation
   **/
  void PressAndHold(const std::shared_ptr<interaction::ATEInteraction>& ate_interaction, const uint64_t& correlation_id,
                    const squish::Object& object) const;

  /**
   * @brief This function performs a release operation to interrupt running pressAndHold() API at the position specified
   * by screen_point in screen global coordinates.
   *
   * Passing argument other than in preceding pressAndHold() call will cause undefined behaviour.
   *
   * @param ate_interaction structure provides the ability to communicate with ATE
   * @param correlation_id - correlation id for RPC
   * @param screen_point Point in absolute coordinates where to perform the release operation
   **/
  void PressRelease(const std::shared_ptr<interaction::ATEInteraction>& ate_interaction, const uint64_t& correlation_id,
                    const common::Point& screen_point) const;

  /**
   * @brief This function performs a release operation to interrupt running pressAndHold() API at the position specified
   * by the center of the screen_rectangle in screen global coordinates.
   *
   * Passing argument other than in preceding pressAndHold() call will cause undefined behaviour.
   *
   * @param ate_interaction structure provides the ability to communicate with ATE
   * @param correlation_id - correlation id for RPC
   * @param screen_rectangle Rectangle in absolute coordinates in the center of which to perform release operation
   **/
  void PressRelease(const std::shared_ptr<interaction::ATEInteraction>& ate_interaction, const uint64_t& correlation_id,
                    const common::Rect& screen_rectangle) const;

  /**
   * @brief This function performs a release operation to interrupt running pressAndHold() API at the position specified
   * by object provided as a result of waitFordObject() API.
   *
   * Passing argument other than in preceding pressAndHold() call will cause undefined behaviour.
   *
   * @param ate_interaction structure provides the ability to communicate with ATE
   * @param correlation_id - correlation id for RPC
   * @param object Object obtained by waitForObject() in the center of which to perform release operation
   **/
  void PressRelease(const std::shared_ptr<interaction::ATEInteraction>& ate_interaction, const uint64_t& correlation_id,
                    const squish::Object& object) const;

  /**
   * @brief This function verifies the object with the symbolic
   * or real (multi-property) name objectName exists on the screen.
   * @param ate_interaction structure provides the ability to communicate with ATE
   * @param correlation_id - correlation id for RPC
   * @param object_name - name of the object for verifying existence on the screen
   * @return returns a true value if the object with the symbolic
   * or real (multi-property) name objectName exists otherwise false
   **/
  bool Exists(const std::shared_ptr<interaction::ATEInteraction>& ate_interaction, const uint64_t& correlation_id,
              const std::string& object_name) const;

  /**
   * @brief The function sets default timeout for WaitForObject function
   * @param timeout - timeout for WaitForObject function
   **/
  void SetDefaultWaitForObjectTimeout(int timeout);

  /**
   * @brief The function allows to check is ApplicationContext already exist, therefore whether a connection was
   * performed
   * @return true in case of ApplicationContext exist, false - otherwise
   **/
  bool IsApplicationContextExist() const;

 private:
  squish::ApplicationContext application_context_;

  int default_wait_for_object_timeout_in_ms_ = 0;
  const int default_long_press_timeout_in_ms_ = 2000;
};
}  // namespace API

#endif  // SQUISH_API_H_
