#ifndef SQUISH_API_H_
#define SQUISH_API_H_

#include <chrono>
#include <string>

#include "utils/geometry_types.h"
#include "utils/squish_types.h"

#include "squish/application_context.h"
#include "squish/squish_types.h"
#include "error_defines.h"

namespace squish {

struct API {
  /**
   * @brief This function causes to attach to the application called aut_name.
   * @param aut_name (ignored) the name of an application that has been registered with the squishserver as an
   *attachable AUT
   * @return A handle to its application context
   **/
  static ApplicationContext AttachToApplication(const std::string& aut_name = "");

  /**
   * @brief WaitForObject waits until the objectOrName object is accessible (i.e., it exists and is visible and
   *enabled). The function waits for the time defined by the testSettings.waitForObjectTimeout property, that many
   *milliseconds
   * @param _objectOrName - desirable object or name
   * @returns the object if successful or raises a (catchable) LookupError exception on failure, i.e., if
   * the function times out
   **/
  static Object WaitForObject(const std::string& object_or_name);
  static Object WaitForObject(const Object& object_or_name);

  /**
   * @brief WaitForObject waits until the objectOrName object is accessible (i.e., it exists and is visible and
   *enabled). The function waits for the time defined by the optional timeoutMSec parameter is used, that many
   *milliseconds. This function is useful if you want to synchronize your script execution.
   * @param _objectOrName - desirable object or name
   * @param timeoutMSec - timeout in miliseconds
   * @returns the object if successful or raises a (catchable) LookupError exception on failure, i.e., if
   * the function times out.
   **/
  static Object WaitForObject(const std::string& object_or_name, int timeout_msec);
  static Object WaitForObject(const Object& object_or_name, int timeout_msec);

  /**
   * @brief TapObject performs a touch tap at the position specified by screenPoint.
   * @param screen_point - position are in screen global coordinates
   * @param The optional modifierState specify keyboard modifiers that are held down during the click
   * @param The optional button specify mouse button that held down during the click
   **/

  static void TapObject(const common::Point& screen_point,
                        common::squish::ModifierState modifier_state = common::squish::ModifierState::NONE,
                        common::squish::MouseButton button = common::squish::MouseButton::NONE);

  /**
   * @brief TapObject performs a touch tap at the center of the rectangle specified by screenRectangle.
   * @param screen_rectangle - position are in screen global coordinates
   * @param The optional modifierState specify keyboard modifiers that are held down during the click
   * @param The optional button specify mouse button that held down during the click
   **/
  static void TapObject(const common::Rect& screen_rectangle,
                        common::squish::ModifierState modifier_state = common::squish::ModifierState::NONE,
                        common::squish::MouseButton button = common::squish::MouseButton::NONE);
  static void TapObject(const Object& screen_rectangle,
                        common::squish::ModifierState modifier_state = common::squish::ModifierState::NONE,
                        common::squish::MouseButton button = common::squish::MouseButton::NONE);
};
}  // namespace squish

#endif  // SQUISH_API_H_