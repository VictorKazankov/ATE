#ifndef ATE_CLIENT_CLIENT_CLI_API_MAPPER_H_
#define ATE_CLIENT_CLIENT_CLI_API_MAPPER_H_

#include "application_context.h"
#include "enums.h"
#include "errors.h"
#include "object.h"
#include "screen_point.h"
#include "screen_rectangle.h"
#include "wildcard.h"

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
      <exception cref="LookupError">Thrown on failure, i.e. if the method times out.</exception>
      <exception cref="VideoStreamingError">Thrown if video stream is not available.</exception>
      <exception cref="NoConnectionEstablished">Thrown if not connected to ATE server.</exception>
      <exception cref="System::ArgumentException">Thrown if one or more of the arguments provided are not valid.</exception>
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
      <exception cref="LookupError">Thrown on failure, i.e. if the method times out.</exception>
      <exception cref="VideoStreamingError">Thrown if video stream is not available.</exception>
      <exception cref="NoConnectionEstablished">Thrown if not connected to ATE server.</exception>
      <exception cref="System::ArgumentException">Thrown if one or more of the arguments provided are not valid.</exception>
      <exception cref="System::ArgumentNullException">Thrown if null passed as argument.</exception>
      <exception cref="System::Exception">Thrown if internal error has occurred.</exception>
   **/
  static ATE::Object^ WaitForObject(System::String^ objectOrName);

  /** <summary>
        waitForObject waits until the first found object that matches <paramref name="wildcard"/> pattern is
        accessible (i.e., it exists and is visible). Method waits for the time defined by the waitForObjectTimeout
        config setting, that many milliseconds.
      </summary>
      <param name="wildcard">Wildcard pattern to match object.</param>
      <returns>First found Object that matches pattern if successful</returns>
      <exception cref="LookupError">Thrown on failure, i.e. if the method times out.</exception>
      <exception cref="VideoStreamingError">Thrown if video stream is not available.</exception>
      <exception cref="NoConnectionEstablished">Thrown if not connected to ATE server.</exception>
      <exception cref="System::ArgumentException">Thrown if one or more of the arguments provided are not valid.</exception>
      <exception cref="System::ArgumentNullException">Thrown if null passed as argument.</exception>
      <exception cref="System::Exception">Thrown if internal error has occurred.</exception>
   **/
  static ATE::Object^ WaitForObject(Wildcard^ wildcard);

  /** <summary>
        waitForObject waits until the <paramref name="objectOrName"/> object is accessible (i.e., it exists and is visible).
        Method waits for the time defined by the <paramref name="timeoutMSec"/>, that many milliseconds.
      </summary>
      <param name="objectOrName">Object or name to wait for.</param>
      <param name="timeoutMSec">Wait timeout in milliseconds.</param>
      <returns>Object if successful</returns>
      <exception cref="LookupError">Thrown on failure, i.e. if the method times out.</exception>
      <exception cref="VideoStreamingError">Thrown if video stream is not available.</exception>
      <exception cref="NoConnectionEstablished">Thrown if not connected to ATE server.</exception>
      <exception cref="System::ArgumentException">Thrown if one or more of the arguments provided are not valid.</exception>
      <exception cref="System::ArgumentNullException">Thrown if null passed as argument.</exception>
      <exception cref="System::Exception">Thrown if internal error has occurred.</exception>
   **/
  static ATE::Object^ WaitForObject(ATE::Object^ objectOrName, int timeoutMSec);

  /** <summary>
        waitForObject waits until the <paramref name="objectOrName"/> object is accessible (i.e., it exists and is visible).
        Method waits for the time defined by the <paramref name="timeoutMSec"/>, that many milliseconds.
      </summary>
      <param name="objectOrName">Object or name to wait for.</param>
      <param name="timeoutMSec">Wait timeout in milliseconds.</param>
      <returns>Object if successful</returns>
      <exception cref="LookupError">Thrown on failure, i.e. if the method times out.</exception>
      <exception cref="VideoStreamingError">Thrown if video stream is not available.</exception>
      <exception cref="NoConnectionEstablished">Thrown if not connected to ATE server.</exception>
      <exception cref="System::ArgumentException">Thrown if one or more of the arguments provided are not valid.</exception>
      <exception cref="System::ArgumentNullException">Thrown if null passed as argument.</exception>
      <exception cref="System::Exception">Thrown if internal error has occurred.</exception>
   **/
  static ATE::Object^ WaitForObject(System::String^ objectOrName, int timeoutMSec);

  /** <summary>
        waitForObject waits until the first found object that matches <paramref name="wildcard"/> pattern
        is accessible (i.e., it exists and is visible). Method waits for the time defined by the
        <paramref name="timeoutMSec"/>, that many milliseconds.
      </summary>
      <param name="wildcard">Wildcard pattern to match object.</param>
      <param name="timeoutMSec">Wait timeout in milliseconds.</param>
      <returns>First found Object that matches pattern if successful</returns>
      <exception cref="LookupError">Thrown on failure, i.e. if the method times out.</exception>
      <exception cref="VideoStreamingError">Thrown if video stream is not available.</exception>
      <exception cref="NoConnectionEstablished">Thrown if not connected to ATE server.</exception>
      <exception cref="System::ArgumentException">Thrown if one or more of the arguments provided are not valid.</exception>
      <exception cref="System::ArgumentNullException">Thrown if null passed as argument.</exception>
      <exception cref="System::Exception">Thrown if internal error has occurred.</exception>
   **/
  static ATE::Object^ WaitForObject(Wildcard^ wildcard, int timeoutMSec);

  /** <summary>
        This method performs a touch tap at the position specified by <paramref name="screenPoint"/> in screen global
        coordinates. The <paramref name="modifierState"/> and <paramref name="button"/> arguments specify keyboard
        modifiers and the mouse button that are held down during the tap.
      </summary>
      <param name="screenPoint">Position in screen global coordinates.</param>
      <param name="modifierState">Argument which indicates which special keys are pressed at the time of a tap.</param>
      <param name="button">Argument which indicates which mouse button is used for a tap.</param>
      <exception cref="NoConnectionEstablished">Thrown if not connected to ATE server.</exception>
      <exception cref="System::ArgumentException">Thrown if one or more of the arguments provided are not valid.</exception>
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
      <exception cref="NoConnectionEstablished">Thrown if not connected to ATE server.</exception>
      <exception cref="System::ArgumentException">Thrown if one or more of the arguments provided are not valid.</exception>
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
      <exception cref="NoConnectionEstablished">Thrown if not connected to ATE server.</exception>
      <exception cref="System::ArgumentException">Thrown if one or more of the arguments provided are not valid.</exception>
      <exception cref="System::ArgumentNullException">Thrown if null passed as argument.</exception>
      <exception cref="System::Exception">Thrown if internal error has occurred.</exception>
   **/
  static void TapObject(ATE::Object^ object, Modifier modifierState, MouseButton button);

  /** <summary>
        This method performs a touch-based drag operation. It initiates a touch drag of the <paramref name="objectOrName"/>
        widget starting at the position <paramref name="x"/> and <paramref name="y"/>, and is dragged by <paramref name="dx"/>
        pixels horizontally and <paramref name="dy"/> pixels vertically.
      </summary>
      <param name="objectOrName">Object to perform drag operation on.</param>
      <param name="x">Drag start x coordinate</param>
      <param name="y">Drag start y coordinate</param>
      <param name="dx">Pixels to drag horizontally</param>
      <param name="dy">Pixels to drag vertically</param>
      <param name="modifierState">Argument which indicates which special keys are pressed at the time of a tap.</param>
      <exception cref="NoConnectionEstablished">Thrown if not connected to ATE server.</exception>
      <exception cref="System::ArgumentException">Thrown if one or more of the arguments provided are not valid.</exception>
      <exception cref="System::ArgumentNullException">Thrown if null passed as argument.</exception>
      <exception cref="System::Exception">Thrown if internal error has occurred.</exception>
   **/
  static void TouchAndDrag(System::String^ objectOrName, int x, int y, int dx, int dy, Modifier modifierState);

  /** <summary>
        This method performs a touch-based drag operation. It initiates a touch drag of the <paramref name="objectOrName"/>
        widget starting at the position <paramref name="x"/> and <paramref name="y"/>, and is dragged by <paramref name="dx"/>
        pixels horizontally and <paramref name="dy"/> pixels vertically.
      </summary>
      <param name="objectOrName">Object to perform drag operation on.</param>
      <param name="x">Drag start x coordinate</param>
      <param name="y">Drag start y coordinate</param>
      <param name="dx">Pixels to drag horizontally</param>
      <param name="dy">Pixels to drag vertically</param>
      <param name="modifierState">Argument which indicates which special keys are pressed at the time of a tap.</param>
      <exception cref="NoConnectionEstablished">Thrown if not connected to ATE server.</exception>
      <exception cref="System::ArgumentException">Thrown if one or more of the arguments provided are not valid.</exception>
      <exception cref="System::ArgumentNullException">Thrown if null passed as argument.</exception>
      <exception cref="System::Exception">Thrown if internal error has occurred.</exception>
   **/
  static void TouchAndDrag(ATE::Object^ objectOrName, int x, int y, int dx, int dy, Modifier modifierState);

  /** <summary>
        This method performs a press and hold operation specified by a <paramref name="screenPoint"/>. Position is in
        screen global coordinates. Press and hold operation doesn't have a timeout for execution and can be interrupted
        by pressRelease() API only. Any subsequent call of any other touch based operation prior calling pressRelease()
        will cause undefined behaviour.
      </summary>
      <param name="screenPoint">Position in screen global coordinates.</param>
      <exception cref="NoConnectionEstablished">Thrown if not connected to ATE server.</exception>
      <exception cref="System::ArgumentException">Thrown if one or more of the arguments provided are not valid.</exception>
      <exception cref="System::Exception">Thrown if internal error has occurred.</exception>
   **/
  static void PressAndHold(ScreenPoint screenPoint);

  /** <summary>
        This method performs a press and hold operation specified by the center of a <paramref name="screenRectangle"/>.
        Position is in screen global coordinates. Press and hold operation doesn't have a timeout for execution and can
        be interrupted by pressRelease() API only. Any subsequent call of any other touch based operation prior calling
        pressRelease() will cause undefined behaviour.
      </summary>
      <param name="screenRectangle">Rectangle, center of which specifies position in screen global coordinates.</param>
      <exception cref="NoConnectionEstablished">Thrown if not connected to ATE server.</exception>
      <exception cref="System::ArgumentException">Thrown if one or more of the arguments provided are not valid.</exception>
      <exception cref="System::ArgumentNullException">Thrown if null passed as argument.</exception>
      <exception cref="System::Exception">Thrown if internal error has occurred.</exception>
   **/
  static void PressAndHold(ScreenRectangle^ screenRectangle);

  /** <summary>
        This method performs a press and hold operation specified by the center of the <paramref name="object"/> obtained
        by waitForObject(). Press and hold operation doesn't have a timeout for execution and can be interrupted by
        pressRelease() API only. Any subsequent call of any other touch based operation prior calling pressRelease() will
        cause undefined behaviour.
      </summary>
      <param name="object">Object, center of which specifies position.</param>
      <exception cref="NoConnectionEstablished">Thrown if not connected to ATE server.</exception>
      <exception cref="System::ArgumentException">Thrown if one or more of the arguments provided are not valid.</exception>
      <exception cref="System::ArgumentNullException">Thrown if null passed as argument.</exception>
      <exception cref="System::Exception">Thrown if internal error has occurred.</exception>
   **/
  static void PressAndHold(ATE::Object^ object);

  /** <summary>
        This method performs a release operation to interrupt running pressAndHold() API at the position specified by
        <paramref name="screenPoint"/> in screen global coordinates. Passing argument other than in preceding pressAndHold()
        call will cause undefined behaviour.
      </summary>
      <param name="screenPoint">Position in screen global coordinates.</param>
      <exception cref="NoConnectionEstablished">Thrown if not connected to ATE server.</exception>
      <exception cref="System::ArgumentException">Thrown if one or more of the arguments provided are not valid.</exception>
      <exception cref="System::Exception">Thrown if internal error has occurred.</exception>
   **/
  static void PressRelease(ScreenPoint screenPoint);

  /** <summary>
        This method performs a release operation to interrupt running pressAndHold() API at the position specified by
        the center of the <paramref name="screenRectangle"/> in screen global coordinates. Passing argument other than
        in preceding pressAndHold() call will cause undefined behaviour.
      </summary>
      <param name="screenRectangle">Rectangle, center of which specifies position in screen global coordinates.</param>
      <exception cref="NoConnectionEstablished">Thrown if not connected to ATE server.</exception>
      <exception cref="System::ArgumentException">Thrown if one or more of the arguments provided are not valid.</exception>
      <exception cref="System::ArgumentNullException">Thrown if null passed as argument.</exception>
      <exception cref="System::Exception">Thrown if internal error has occurred.</exception>
   **/
  static void PressRelease(ScreenRectangle^ screenRectangle);

  /** <summary>
        This method performs a release operation to interrupt running pressAndHold() API at the position specified by
        <paramref name="object"/> provided as a result of waitForObject() API. Passing argument other than in preceding
        pressAndHold() call will cause undefined behaviour.
      </summary>
      <param name="object">Object, center of which specifies position.</param>
      <exception cref="NoConnectionEstablished">Thrown if not connected to ATE server.</exception>
      <exception cref="System::ArgumentException">Thrown if one or more of the arguments provided are not valid.</exception>
      <exception cref="System::ArgumentNullException">Thrown if null passed as argument.</exception>
      <exception cref="System::Exception">Thrown if internal error has occurred.</exception>
   **/
  static void PressRelease(ATE::Object^ object);

   /** <summary>
        This method sets the <paramref name="syncVersion"/> and <paramref name="syncBuildVersion"/> of current Icon collection.
      </summary>
      <param name="syncVersion">SYNC product name.</param>
      <param name="syncBuildVersion">SYNC build revision.</param>
      <exception cref="InvalidSyncVersion">Thrown if selected SYNC product name is invalid.</exception>
      <exception cref="InvalidSyncBuildVersion">Thrown if selected SYNC build revision is invalid.</exception>
      <exception cref="NoConnectionEstablished">Thrown if not connected to ATE server.</exception>
      <exception cref="System::ArgumentException">Thrown if one or more of the arguments provided are not valid.</exception>
      <exception cref="System::ArgumentNullException">Thrown if null passed as argument.</exception>
      <exception cref="System::Exception">Thrown if internal error has occurred.</exception>
   **/
  static void ChangeSyncIconDB(System::String^ syncVersion, System::String^ syncBuildVersion);

   /** <summary>
        This method sets the <paramref name="collectionMode"/> of current Icon collection.
      </summary>
      <param name="collectionMode">HMI light mode.</param>
      <exception cref="InvalidSyncCollectionMode">Thrown if selected HMI light mode is invalid.</exception>
      <exception cref="NoConnectionEstablished">Thrown if not connected to ATE server.</exception>
      <exception cref="System::ArgumentException">Thrown if one or more of the arguments provided are not valid.</exception>
      <exception cref="System::ArgumentNullException">Thrown if null passed as argument.</exception>
      <exception cref="System::Exception">Thrown if internal error has occurred.</exception>
   **/
  static void ChangeSyncMode(CollectionMode collectionMode);

  /** <summary>
        This method presses the center of the <paramref name="object"/> for <paramref name="timeoutMSec"/> milliseconds.
        <paramref name="timeoutMSec"/> cannot exceed 60 seconds.
      </summary>
      <param name="object">Object to press</param>
      <param name="timeoutMSec">Milliseconds to hold press</param>
      <exception cref="InvalidDuration">Thrown if press duration is longer than 60 seconds.</exception>
      <exception cref="NoConnectionEstablished">Thrown if not connected to ATE server.</exception>
      <exception cref="System::ArgumentException">Thrown if one or more of the arguments provided are not valid.</exception>
      <exception cref="System::ArgumentNullException">Thrown if null passed as argument.</exception>
      <exception cref="System::Exception">Thrown if internal error has occurred.</exception>
   **/
  static void LongPress(ATE::Object^ object, int timeoutMSec);

  /** <summary>
        This method presses the <paramref name="object"/> for <paramref name="timeoutMSec"/> milliseconds in the point
        (<paramref name="x"/>, <paramref name="y"/>) relative to <paramref name="object"/>. <paramref name="timeoutMSec"/>
        cannot exceed 60 seconds.
      </summary>
      <param name="object">Object to press</param>
      <param name="x">x coordinate of press relative to object</param>
      <param name="y">y coordinate of press relative to object</param>
      <param name="timeoutMSec">Milliseconds to hold press</param>
      <exception cref="InvalidDuration">Thrown if press duration is longer than 60 seconds.</exception>
      <exception cref="NoConnectionEstablished">Thrown if not connected to ATE server.</exception>
      <exception cref="System::ArgumentException">Thrown if one or more of the arguments provided are not valid.</exception>
      <exception cref="System::ArgumentNullException">Thrown if null passed as argument.</exception>
      <exception cref="System::Exception">Thrown if internal error has occurred.</exception>
   **/
  static void LongPress(ATE::Object^ object, int x, int y, int timeoutMSec);

  /** <summary>
        This method captures and saves screenshot from connected SYNC device.
      </summary>
      <param name="fileName">The screenshot file name. Must have 'png' extension.</param>
      <param name="location">Subdirectory where to store screenshot relative to '/var/lib/vdp/vhat' directory on VDP board.</param>
      <returns>true if successful</returns>
      <exception cref="VideoStreamingError">Thrown if video stream is not available.</exception>
      <exception cref="EmptyScreenshotFileName">Thrown if screenshot file name is empty.</exception>
      <exception cref="WrongScreenshotExtension">Thrown if screenshot extension is invalid.</exception>
      <exception cref="PermissionDenied">Thrown if permission denied.</exception>
      <exception cref="ImageAssemblingFailed">Thrown if failed to save screenshot.</exception>
      <exception cref="NoAvailableDiskSpace">Thrown if out of disk space.</exception>
      <exception cref="NoConnectionEstablished">Thrown if not connected to ATE server.</exception>
      <exception cref="System::ArgumentException">Thrown if one or more of the arguments provided are not valid.</exception>
      <exception cref="System::ArgumentNullException">Thrown if null passed as argument.</exception>
      <exception cref="System::Exception">Thrown if internal error has occurred.</exception>
   **/
  static bool GetScreenshot(System::String^ fileName, System::String^ location);

  /** <summary>
        Method returns the text in rectangle specified by coordinates.
      </summary>
      <param name="x1">Rectangle left coordinate.</param>
      <param name="y1">Rectangle top coordinate.</param>
      <param name="x2">Rectangle right coordinate.</param>
      <param name="y2">Rectangle bottom coordinate.</param>
      <returns>List of Objects.</returns>
      <exception cref="VideoStreamingError">Thrown if video stream is not available.</exception>
      <exception cref="InvalidRectangleCoordinates">Thrown if top-left and bottom-right coordinates are mixed up or
                                                    rectangle has zero height/width or is out of frame boundaries</exception>
      <exception cref="NoConnectionEstablished">Thrown if not connected to ATE server.</exception>
      <exception cref="System::ArgumentException">Thrown if one or more of the arguments provided are not valid.</exception>
      <exception cref="System::ArgumentNullException">Thrown if null passed as argument.</exception>
      <exception cref="System::Exception">Thrown if internal error has occurred.</exception>
   **/
  static System::String^ GetText(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

  /** <summary>
        This method provides a list of objects from Icon database that match specified <paramref name="name"/>.
      </summary>
      <param name="name">Name to match.</param>
      <returns>List of Objects.</returns>
      <exception cref="NoConnectionEstablished">Thrown if not connected to ATE server.</exception>
      <exception cref="System::ArgumentException">Thrown if one or more of the arguments provided are not valid.</exception>
      <exception cref="System::ArgumentNullException">Thrown if null passed as argument.</exception>
      <exception cref="System::Exception">Thrown if internal error has occurred.</exception>
   **/
  static System::Collections::Generic::List<ATE::Object^>^ GetObjectsDataByPattern(System::String^ name);

  /** <summary>
        This method provides a list of objects from Icon database that match <paramref name="wildcard"/> pattern.
      </summary>
      <param name="wildcard">Pattern to match.</param>
      <exception cref="NoConnectionEstablished">Thrown if not connected to ATE server.</exception>
      <exception cref="System::ArgumentException">Thrown if one or more of the arguments provided are not valid.</exception>
      <exception cref="System::ArgumentNullException">Thrown if null passed as argument.</exception>
      <exception cref="System::Exception">Thrown if internal error has occurred.</exception>
   **/
  static System::Collections::Generic::List<ATE::Object^>^ GetObjectsDataByPattern(Wildcard^ wildcard);

  /** <summary>
        This methods compares two images and returns the percentage of discrepancy.
      </summary>
      <param name="pathSecond">Path to second image.</param>
      <param name="pathFirst">Path to first image.</param>
      <returns>Percentage of discrepancy of two images.</returns>
      <exception cref="ComparingImageIncorrectSize">Thrown if image size is incorrect.</exception>
      <exception cref="ComparingImageNotExist">Thrown if compared image does not exist.</exception>
      <exception cref="UnsupportedFileType">Thrown if image format is not supported (supported format are: 'png', 'jpeg', 'jpg').</exception>
      <exception cref="EmptyScreenshotFileName">Thrown if screenshot file name is empty.</exception>
      <exception cref="PermissionDenied">Thrown if permission denied.</exception>
      <exception cref="NoConnectionEstablished">Thrown if not connected to ATE server.</exception>
      <exception cref="System::ArgumentException">Thrown if one or more of the arguments provided are not valid.</exception>
      <exception cref="System::ArgumentNullException">Thrown if null passed as argument.</exception>
      <exception cref="System::Exception">Thrown if internal error has occurred.</exception>
   **/
  static int GetImagesDiscrepancy(System::String^ pathSecond, System::String^ pathFirst);


  /** <summary>
        This methods compares two images and returns the percentage of discrepancy. <paramref name="topLeftCoordinate"/>
        and <paramref name="bottomRightCoordinate"/> specify a sub-area of image to compare.
      </summary>
      <param name="pathSecond">Path to second image.</param>
      <param name="pathFirst">Path to first image.</param>
      <param name="topLeftCoordinate">Coordinates of top-left point of comparison area.</param>
      <param name="bottomRightCoordinate">Coordinates of bottom-right point of comparison area.</param>
      <returns>Percentage of discrepancy of two images.</returns>
      <exception cref="InvalidRectangleCoordinates">Thrown if top-left and bottom-right coordinates are mixed up or
                                                    rectangle has zero height/width or is out of frame boundaries.</exception>
      <exception cref="ComparingImageIncorrectSize">Thrown if image size is incorrect.</exception>
      <exception cref="ComparingImageNotExist">Thrown if compared image does not exist.</exception>
      <exception cref="UnsupportedFileType">Thrown if image format is not supported (supported format are: 'png', 'jpeg', 'jpg').</exception>
      <exception cref="EmptyScreenshotFileName">Thrown if screenshot file name is empty.</exception>
      <exception cref="PermissionDenied">Thrown if permission denied.</exception>
      <exception cref="NoConnectionEstablished">Thrown if not connected to ATE server.</exception>
      <exception cref="System::ArgumentException">Thrown if one or more of the arguments provided are not valid.</exception>
      <exception cref="System::ArgumentNullException">Thrown if null passed as argument.</exception>
      <exception cref="System::Exception">Thrown if internal error has occurred.</exception>
   **/
  static int GetImagesDiscrepancy(System::String^ pathSecond, System::String^ pathFirst,
                                  ScreenPoint topLeftCoordinate, ScreenPoint bottomRightCoordinate);

  /** <summary>
        Capture frames of desired area for specified <paramref name="duration"/> at a specified <paramref name="interval"/>.
        Maximum <paramref name="duration"/> is 5 seconds (5 000 milliseconds).
      </summary>
      <param name="interval">Capture interval in milliseconds.</param>
      <param name="duration">Capture duration in milliseconds.</param>
      <param name="top_left">Top-left coordinate of capture area.</param>
      <param name="bottom_right">Bottom-right coordinate of capture area.</param>
      <param name="path">Directory where to store frames.</param>
      <returns>List of frame names</returns>
      <exception cref="InvalidDuration">Thrown if duration is longer than 5 seconds.</exception>
      <exception cref="InvalidRectangleCoordinates">Thrown if top-left and bottom-right coordinates are mixed up or
                                                    rectangle has zero height/width or is out of frame boundaries.</exception>
      <exception cref="VideoStreamingError">Thrown if video stream is not available.</exception>
      <exception cref="PermissionDenied">Thrown if permission denied.</exception>
      <exception cref="ImageAssemblingFailed">Thrown if failed to save captured frame.</exception>
      <exception cref="NoAvailableDiskSpace">Thrown if out of disk space.</exception>
      <exception cref="NoConnectionEstablished">Thrown if not connected to ATE server.</exception>
      <exception cref="System::ArgumentException">Thrown if one or more of the arguments provided are not valid.</exception>
      <exception cref="System::ArgumentNullException">Thrown if null passed as argument.</exception>
      <exception cref="System::Exception">Thrown if internal error has occurred.</exception>
   **/
  static System::Collections::Generic::List<System::String^>^ CaptureFrames(int interval, int duration, ScreenPoint top_left,
                                                                            ScreenPoint bottom_right, System::String^ path);
};

// clang-format on

}  // namespace ATE

#endif  // ATE_CLIENT_CLIENT_CLI_API_MAPPER_H_
