#ifndef ATE_CLIENT_CLIENT_CLI_API_MAPPER_H_
#define ATE_CLIENT_CLIENT_CLI_API_MAPPER_H_

#include "application_context.h"
#include "enums.h"
#include "errors.h"
#include "object.h"
#include "screen_point.h"
#include "screen_rectangle.h"

namespace ATE {

// clang-format off

public ref class API {
 public:
  /** <summary>
        This method causes ATE client to attach to the application called
        <paramref name="autName"/> and returns a handle to its application
        context.
      </summary>
      <param name="autName">Name of an application to attach to (temporarily ignored by ATE).</param>
      <returns>Handle to attached application context.</returns>
      <exception cref="System::ArgumentNullException">Thrown if null passed as argument.</exception>
      <exception cref="System::Exception">Thrown if cannot connect to ATE server or internal error has occurred.</exception>
   **/
  static ApplicationContext^ AttachToApplication(System::String^ autName);

  /** <summary>
        waitForObject waits until the <paramref name="objectOrName"/> object is accessible (i.e., it exists and is visible).
        Method waits for the time defined by the waitForObjectTimeout config setting, that many milliseconds.
      </summary>
      <param name="objectOrName">Object or name to wait for.</param>
      <returns>Object if successful</returns>
      <exception cref="LookupError">Thrown on failure, i.e. if the method times out</exception>
      <exception cref="VideoStreamingError">Thrown if video stream is not available</exception>
      <exception cref="NoConnectionEstablished">Thrown if not connected to ATE server</exception>
      <exception cref="System::ArgumentException">Thrown if one or more of the arguments provided are not valid</exception>
      <exception cref="System::ArgumentNullException">Thrown if null passed as argument.</exception>
      <exception cref="System::Exception">Thrown if internal error has occurred.</exception>
   **/
  static ATE::Object^ WaitForObject(ATE::Object^ objectOrName);

  /** <summary>
        waitForObject waits until the <paramref name="objectOrName"/> object is accessible (i.e., it exists and is visible).
        Method waits for the time defined by the waitForObjectTimeout config setting, that many milliseconds.
      </summary>
      <param name="objectOrName">Object or name to wait for.</param>
      <returns>Object if successful</returns>
      <exception cref="LookupError">Thrown on failure, i.e. if the method times out</exception>
      <exception cref="VideoStreamingError">Thrown if video stream is not available</exception>
      <exception cref="NoConnectionEstablished">Thrown if not connected to ATE server</exception>
      <exception cref="System::ArgumentException">Thrown if one or more of the arguments provided are not valid</exception>
      <exception cref="System::ArgumentNullException">Thrown if null passed as argument.</exception>
      <exception cref="System::Exception">Thrown if internal error has occurred.</exception>
   **/
  static ATE::Object^ WaitForObject(System::String^ objectOrName);

  /** <summary>
        waitForObject waits until the <paramref name="objectOrName"/> object is accessible (i.e., it exists and is visible).
        Method waits for the time defined by the <paramref name="timeoutMSec"/>, that many milliseconds.
      </summary>
      <param name="objectOrName">Object or name to wait for.</param>
      <returns>Object if successful</returns>
      <exception cref="LookupError">Thrown on failure, i.e. if the method times out</exception>
      <exception cref="VideoStreamingError">Thrown if video stream is not available</exception>
      <exception cref="NoConnectionEstablished">Thrown if not connected to ATE server</exception>
      <exception cref="System::ArgumentException">Thrown if one or more of the arguments provided are not valid</exception>
      <exception cref="System::ArgumentNullException">Thrown if null passed as argument.</exception>
      <exception cref="System::Exception">Thrown if internal error has occurred.</exception>
   **/
  static ATE::Object^ WaitForObject(ATE::Object^ objectOrName, int timeoutMSec);

  /** <summary>
        waitForObject waits until the <paramref name="objectOrName"/> object is accessible (i.e., it exists and is visible).
        Method waits for the time defined by the <paramref name="timeoutMSec"/>, that many milliseconds.
      </summary>
      <param name="objectOrName">Object or name to wait for.</param>
      <returns>Object if successful</returns>
      <exception cref="LookupError">Thrown on failure, i.e. if the method times out</exception>
      <exception cref="VideoStreamingError">Thrown if video stream is not available</exception>
      <exception cref="NoConnectionEstablished">Thrown if not connected to ATE server</exception>
      <exception cref="System::ArgumentException">Thrown if one or more of the arguments provided are not valid</exception>
      <exception cref="System::ArgumentNullException">Thrown if null passed as argument.</exception>
      <exception cref="System::Exception">Thrown if internal error has occurred.</exception>
   **/
  static ATE::Object^ WaitForObject(System::String^ objectOrName, int timeoutMSec);

  /** <summary>
        This method performs a touch tap at the position specified by <paramref name="screenPoint"/> in screen global
        coordinates. The <paramref name="modifierState"/> and <paramref name="button"/> arguments specify keyboard
        modifiers and the mouse button that are held down during the tap.
      </summary>
      <param name="screenPoint">Position in screen global coordinates.</param>
      <param name="modifierState">Argument which indicates which special keys are pressed at the time of a tap.</param>
      <param name="button">Argument which indicates which mouse button is used for a tap.</param>
      <exception cref="NoConnectionEstablished">Thrown if not connected to ATE server</exception>
      <exception cref="System::ArgumentException">Thrown if one or more of the arguments provided are not valid</exception>
      <exception cref="System::Exception">Thrown if internal error has occurred.</exception>
   **/
  static void TapObject(ScreenPoint screenPoint, Modifier modifierState, MouseButton button);

  /** <summary>
        This method performs a touch tap at the center of the rectangle specified by <paramref name="screenRectangle"/>
        in screen global coordinates. The <paramref name="modifierState"/> and <paramref name="button"/> arguments
        specify keyboard modifiers and the mouse button that are held down during the tap.
      </summary>
      <param name="screenRectangle">Rectangle, center of which to be tapped in screen global coordinates.</param>
      <param name="modifierState">Argument which indicates which special keys are pressed at the time of a tap.</param>
      <param name="button">Argument which indicates which mouse button is used for a tap.</param>
      <exception cref="NoConnectionEstablished">Thrown if not connected to ATE server</exception>
      <exception cref="System::ArgumentException">Thrown if one or more of the arguments provided are not valid</exception>
      <exception cref="System::ArgumentNullException">Thrown if null passed as argument.</exception>
      <exception cref="System::Exception">Thrown if internal error has occurred.</exception>
   **/
  static void TapObject(ScreenRectangle^ screenRectangle, Modifier modifierState, MouseButton button);

  /** <summary>
        This method performs a touch tap at the center of the bounding rectangle of the <paramref name="object"/>
        in screen global coordinates. The <paramref name="modifierState"/> and <paramref name="button"/> arguments
        specify keyboard modifiers and the mouse button that are held down during the tap.
      </summary>
      <param name="object">Object, center of which to be tapped in screen global coordinates.</param>
      <param name="modifierState">Argument which indicates which special keys are pressed at the time of a tap.</param>
      <param name="button">Argument which indicates which mouse button is used for a tap.</param>
      <exception cref="NoConnectionEstablished">Thrown if not connected to ATE server</exception>
      <exception cref="System::ArgumentException">Thrown if one or more of the arguments provided are not valid</exception>
      <exception cref="System::ArgumentNullException">Thrown if null passed as argument.</exception>
      <exception cref="System::Exception">Thrown if internal error has occurred.</exception>
   **/
  static void TapObject(ATE::Object^ object, Modifier modifierState, MouseButton button);
};

// clang-format on

}  // namespace ATE

#endif  // ATE_CLIENT_CLIENT_CLI_API_MAPPER_H_
