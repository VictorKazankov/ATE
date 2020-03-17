#ifndef SQUISH_API_H_
#define SQUISH_API_H_

#include <chrono>
#include <string>

#include "utils/geometry_types.h"
#include "utils/squish_types.h"

#include "error_defines.h"
#include "interaction.h"
#include "squish/application_context.h"
#include "squish/squish_types.h"
#include "squish/wildcard.h"

namespace API {

/**
 * @brief SquishApi class represents squish-like APIs which can be processed by ATE.
 **/
class SquishApi {
 public:
  /**
   * @brief This function causes to attach to the application called aut_name.
   * @param ate_interaction Structure provides the ability to communicate with ATE
   * @param aut_name (ignored) the name of an application that has been registered with the squish server as an
   *                 attachable AUT
   * @return A handle to its application context
   **/
  squish::ApplicationContext& AttachToApplication(const std::shared_ptr<interaction::Interaction>& ate_interaction,
                                                  const std::string& aut_name = "");

  /**
   * @brief WaitForObject waits until the object_or_name object is accessible (i.e., it exists and is visible and
   *        enabled). The function waits for the time defined by the testSettings.waitForObjectTimeout property, that
   *        many milliseconds
   * @param ate_interaction Structure provides the ability to communicate with ATE
   * @param correlation_id Correlation id for RPC
   * @param object_or_name Desirable object or name
   * @returns The object if successful or raises an exception on failure.
   * @throw LookupError In case of the pattern is not detected on the screen or timeout has expired
   * @throw VideoStreamingError In case of the video stream is not available
   * @throw invalid_argument In case of the invalid arguments in request
   * @throw runtime_error In case of an internal error, parse error, invalid request, a method not found
   **/
  squish::Object WaitForObject(const std::shared_ptr<interaction::Interaction>& ate_interaction,
                               const uint64_t& correlation_id, const std::string& object_or_name) const;
  squish::Object WaitForObject(const std::shared_ptr<interaction::Interaction>& ate_interaction,
                               const uint64_t& correlation_id, const squish::Object& object_or_name) const;

  /**
   * @brief WaitForObject waits until the object_or_name object is accessible (i.e., it exists and is visible and
   *        enabled). The function waits for the time defined by the testSettings.waitForObjectTimeout property, that
   *        many milliseconds
   * @param ate_interaction Structure provides the ability to communicate with ATE
   * @param correlation_id Correlation id for RPC
   * @param wildcard Pattern for selection
   * @returns The object if successful or raises an exception on failure.
   * @throw LookupError In case of the pattern is not detected on the screen or timeout has expired
   * @throw VideoStreamingError In case of the video stream is not available
   * @throw invalid_argument In case of the invalid arguments in request
   * @throw runtime_error In case of an internal error, parse error, invalid request, a method not found
   **/
  squish::Object WaitForObject(const std::shared_ptr<interaction::Interaction>& ate_interaction,
                               const uint64_t& correlation_id, const squish::Wildcard& wildcard) const;

  /**
   * @brief WaitForObject waits until the object_or_name object is accessible (i.e., it exists and is visible and
   *        enabled). The function waits for the time defined by the optional timeoutMSec parameter is used, that many
   *        milliseconds. This function is useful if you want to synchronize your script execution.
   * @param ate_interaction Structure provides the ability to communicate with ATE
   * @param correlation_id Correlation id for RPC
   * @param object_or_name Desirable object or name
   * @param timeoutMSec Timeout in miliseconds
   * @returns The object if successful or raises an exception on failure.
   * @throw LookupError In case of the pattern is not detected on the screen or timeout has expired
   * @throw VideoStreamingError In case of the video stream is not available
   * @throw invalid_argument In case of the invalid arguments in request
   * @throw runtime_error In case of an internal error, parse error, invalid request, a method not found
   **/
  squish::Object WaitForObject(const std::shared_ptr<interaction::Interaction>& ate_interaction,
                               const uint64_t& correlation_id, const std::string& object_or_name,
                               int timeout_msec) const;
  squish::Object WaitForObject(const std::shared_ptr<interaction::Interaction>& ate_interaction,
                               const uint64_t& correlation_id, const squish::Object& object_or_name,
                               int timeout_msec) const;

  /**
   * @brief WaitForObject waits until the object_or_name object is accessible (i.e., it exists and is visible and
   *        enabled). The function waits for the time defined by the optional timeoutMSec parameter is used, that many
   *        milliseconds. This function is useful if you want to synchronize your script execution.
   * @param ate_interaction Structure provides the ability to communicate with ATE
   * @param correlation_id Correlation id for RPC
   * @param wildcard Pattern for selection
   * @param timeoutMSec Timeout in miliseconds
   * @returns The object if successful or raises an exception on failure.
   * @throw LookupError In case of the pattern is not detected on the screen or timeout has expired
   * @throw VideoStreamingError In case of the video stream is not available
   * @throw invalid_argument In case of the invalid arguments in request
   * @throw runtime_error In case of an internal error, parse error, invalid request, a method not found
   **/
  squish::Object WaitForObject(const std::shared_ptr<interaction::Interaction>& ate_interaction,
                               const uint64_t& correlation_id, const squish::Wildcard& wildcard,
                               int timeout_msec) const;

  /**
   * @brief TapObject performs a touch tap at the position specified by screenPoint.
   * @param ate_interaction Structure provides the ability to communicate with ATE
   * @param correlation_id Correlation id for RPC
   * @param screen_point Position are in screen global coordinates
   * @param The optional modifierState specify keyboard modifiers that are held down during the click
   * @param The optional button specify mouse button that held down during the click
   * @throw invalid_argument In case of the invalid arguments in request
   * @throw runtime_error In case of an internal error, parse error, invalid request, a method not found
   **/

  void TapObject(const std::shared_ptr<interaction::Interaction>& ate_interaction, const uint64_t& correlation_id,
                 const common::Point& screen_point,
                 common::squish::ModifierState modifier_state = common::squish::ModifierState::kNone,
                 common::squish::MouseButton button = common::squish::MouseButton::kNone) const;

  /**
   * @brief TapObject performs a touch tap at the center of the rectangle specified by screenRectangle.
   * @param ate_interaction Structure provides the ability to communicate with ATE
   * @param correlation_id Correlation id for RPC
   * @param screen_rectangle Position are in screen global coordinates
   * @param The optional modifierState specify keyboard modifiers that are held down during the click
   * @param The optional button specify mouse button that held down during the click
   * @throw invalid_argument In case of the invalid arguments in request
   * @throw runtime_error In case of an internal error, parse error, invalid request, a method not found
   **/
  void TapObject(const std::shared_ptr<interaction::Interaction>& ate_interaction, const uint64_t& correlation_id,
                 const common::Rect& screen_rectangle,
                 common::squish::ModifierState modifier_state = common::squish::ModifierState::kNone,
                 common::squish::MouseButton button = common::squish::MouseButton::kNone) const;
  void TapObject(const std::shared_ptr<interaction::Interaction>& ate_interaction, const uint64_t& correlation_id,
                 const squish::Object& screen_rectangle,
                 common::squish::ModifierState modifier_state = common::squish::ModifierState::kNone,
                 common::squish::MouseButton button = common::squish::MouseButton::kNone) const;

  /**
   * @brief This function is pressing on the specified object a pointed amount of milliseconds, or 2 seconds if timeout
   *        not specified. The x and y coordinates are optional. If they are not specified the tap is made in the center
   *        of the widget. On the other hand, if the additional parameters are given, the tap is made at position x and
   *        y (in the object coordinates).
   * @param ate_interaction Structure provides the ability to communicate with ATE
   * @param correlation_id Correlation id for RPC
   * @param screen_rectangle Position are in screen global coordinates
   * @param x Relative coordinates in the object, optional
   * @param y Relative coordinates in the object, optional
   * @param timeout_msec Timeout in milliseconds between press event and release event, optional
   * @throw InvalidDurationLongPress In case of the press is longer than 60 seconds.
   * @throw invalid_argument In case of the invalid arguments in request
   * @throw runtime_error In case of an internal error, parse error, invalid request, a method not found
   **/
  void LongPress(const std::shared_ptr<interaction::Interaction>& ate_interaction, const uint64_t& correlation_id,
                 const squish::Object& screen_rectangle, int timeout_msec) const;
  void LongPress(const std::shared_ptr<interaction::Interaction>& ate_interaction, const uint64_t& correlation_id,
                 const squish::Object& screen_rectangle, int x, int y, int timeout_msec) const;

  /**
   * @brief TouchAndDrag performs a touch-based drag operation. It initiates a touch drag of the specified objectOrName
   *        widget starting at position x, y(in the objectOrName widget's coordinates). The objectOrName widge is
   *        dragged by dx pixels horizontally and by dy pixels vertically.
   * @param ate_interaction Structure provides the ability to communicate with ATE
   * @param correlation_id Correlation id for RPC
   * @param object_or_name Desirable object or name
   * @param x Start x coordinate of drag event
   * @param y Start y coordinate of drag event
   * @param dx Dragged by pixels horizontally
   * @param dy Dragged by pixels vertically
   * @param modifier_state Modifier state [NONE, ALT, CONTROL, SHIFT]
   * @throw invalid_argument In case of the invalid arguments in request
   * @throw runtime_error In case of an internal error, parse error, invalid request, a method not found
   **/
  void TouchAndDrag(const std::shared_ptr<interaction::Interaction>& ate_interaction, const uint64_t& correlation_id,
                    const squish::Object& object_or_name, int x, int y, int dx, int dy,
                    common::squish::ModifierState modifier_state) const;
  void TouchAndDrag(const std::shared_ptr<interaction::Interaction>& ate_interaction, const uint64_t& correlation_id,
                    const std::string& object_or_name, int x, int y, int dx, int dy,
                    common::squish::ModifierState modifier_state) const;

  /**
   * @brief This function performs a press and hold operation specified by a screen_point. Position is in screen global
   *        coordinates. Press and hold operation doesn't have a timeout for execution and can be interrupted by
   *        pressRelease() API only.
   *
   * Any subsequent call of any other touch based operation prior calling pressRelease() will cause undefined behaviour.
   *
   * @param ate_interaction Structure provides the ability to communicate with ATE
   * @param correlation_id Correlation id for RPC
   * @param screen_point Point in absolute coordinates where to perform the press operation
   * @throw invalid_argument In case of the invalid arguments in request
   * @throw runtime_error In case of an internal error, parse error, invalid request, a method not found
   **/
  void PressAndHold(const std::shared_ptr<interaction::Interaction>& ate_interaction, const uint64_t& correlation_id,
                    const common::Point& screen_point) const;

  /**
   * @brief This function performs a press and hold operation specified by the center of a screen_rectangle. Position
   *        is in screen global coordinates. Press and hold operation doesn't have a timeout for execution and can be
   *        interrupted by pressRelease() API only.
   *
   * Any subsequent call of any other touch based operation prior calling pressRelease() will cause undefined behaviour.
   *
   * @param ate_interaction Structure provides the ability to communicate with ATE
   * @param correlation_id Correlation id for RPC
   * @param screen_rectangle Rectangle in absolute coordinates in the center of which to perform press operation
   **/
  void PressAndHold(const std::shared_ptr<interaction::Interaction>& ate_interaction, const uint64_t& correlation_id,
                    const common::Rect& screen_rectangle) const;

  /**
   * @brief This function performs a press and hold operation specified by the center of the object obtained by
   *        waitForObject(). Press and hold operation doesn't have a timeout for execution and can be interrupted by
   *        pressRelease() API only.
   *
   * Any subsequent call of any other touch based operation prior calling pressRelease() will cause undefined behaviour.
   *
   * @param ate_interaction Structure provides the ability to communicate with ATE
   * @param correlation_id Correlation id for RPC
   * @param object Object obtained by waitForObject() in the center of which to perform press operation
   **/
  void PressAndHold(const std::shared_ptr<interaction::Interaction>& ate_interaction, const uint64_t& correlation_id,
                    const squish::Object& object) const;

  /**
   * @brief This function performs a release operation to interrupt running pressAndHold() API at the position specified
   *        by screen_point in screen global coordinates.
   *
   * Passing argument other than in preceding pressAndHold() call will cause undefined behaviour.
   *
   * @param ate_interaction Structure provides the ability to communicate with ATE
   * @param correlation_id Correlation id for RPC
   * @param screen_point Point in absolute coordinates where to perform the release operation
   * @throw invalid_argument In case of the invalid arguments in request
   * @throw runtime_error In case of an internal error, parse error, invalid request, a method not found
   **/
  void PressRelease(const std::shared_ptr<interaction::Interaction>& ate_interaction, const uint64_t& correlation_id,
                    const common::Point& screen_point) const;

  /**
   * @brief This function performs a release operation to interrupt running pressAndHold() API at the position specified
   *        by the center of the screen_rectangle in screen global coordinates.
   *
   * Passing argument other than in preceding pressAndHold() call will cause undefined behaviour.
   *
   * @param ate_interaction Structure provides the ability to communicate with ATE
   * @param correlation_id Correlation id for RPC
   * @param screen_rectangle Rectangle in absolute coordinates in the center of which to perform release operation
   * @throw invalid_argument In case of the invalid arguments in request
   * @throw runtime_error In case of an internal error, parse error, invalid request, a method not found
   **/
  void PressRelease(const std::shared_ptr<interaction::Interaction>& ate_interaction, const uint64_t& correlation_id,
                    const common::Rect& screen_rectangle) const;

  /**
   * @brief This function performs a release operation to interrupt running pressAndHold() API at the position specified
   *        by object provided as a result of waitFordObject() API.
   *
   * Passing argument other than in preceding pressAndHold() call will cause undefined behaviour.
   *
   * @param ate_interaction Structure provides the ability to communicate with ATE
   * @param correlation_id Correlation id for RPC
   * @param object Object obtained by waitForObject() in the center of which to perform release operation
   * @throw invalid_argument In case of the invalid arguments in request
   * @throw runtime_error In case of an internal error, parse error, invalid request, a method not found
   **/
  void PressRelease(const std::shared_ptr<interaction::Interaction>& ate_interaction, const uint64_t& correlation_id,
                    const squish::Object& object) const;

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
  bool Exists(const std::shared_ptr<interaction::Interaction>& ate_interaction, const uint64_t& correlation_id,
              const std::string& object_name) const;

  /**
   * @brief The function sets default timeout for WaitForObject function
   * @param timeout Timeout for WaitForObject function
   **/
  void SetDefaultWaitForObjectTimeout(int timeout);

  /**
   * @brief The function allows to check is ApplicationContext already exist, therefore whether a connection was
   *        performed
   * @return True in case of ApplicationContext exist, false - otherwise
   **/
  bool IsApplicationContextExist() const;

 private:
  squish::ApplicationContext application_context_;

  int default_wait_for_object_timeout_in_ms_ = 0;
  const int default_long_press_timeout_in_ms_ = 2000;
};
}  // namespace API

#endif  // SQUISH_API_H_
