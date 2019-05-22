#ifndef SQUISH_H_
#define SQUISH_H_

#include "chrono"
#include "map"

namespace squish {

/**
 * @brief ScreenPoint it is a point element representing a position on screen.
 **/
struct ScreenPoint {
  int x;
  int y;
};

/**
 * @brief ScreenRectangle it is a rectangle element with screen position and size properties.
 **/
struct ScreenRectangle {
  int x;
  int y;
  int width;
  int height;

  ScreenPoint Center() const;
};

using Object = std::map<std::string, std::string>;

struct ApplicationContext {
  std::string host_;
  size_t port;

  /**
   * @brief This function detaches from a previously started (or attached) application.
   * If the respective application has been started with startApplication Squish will attempt to close it, and
   * eventually (if still running) forcefully close it.
   *
   * If the respective application has been attached to with attachToApplication Squish will leave it running but detach
   * from it, thus leaving the application running. The application can then be attached to with attachToApplication
   * again.
   *
   * After detaching from an application one should not access any of its objects anymore. (This would typically be done
   * by using object references which had been retrieved before detaching.)
   **/
  void Detach();
};

namespace API {

/**
 * @brief This function causes to attach to the application called aut_name.
 * @param aut_name (ignored) the name of an application that has been registered with the squishserver as an attachable AUT
 * @return A handle to its application context
 **/
ApplicationContext AttachToApplication(const std::string aut_name = "");

/**
 * @brief WaitForObject waits until the objectOrName object is accessible (i.e., it exists and is visible and enabled).
 * The function waits for the time defined by the testSettings.waitForObjectTimeout property, that many milliseconds
 * @param _objectOrName - desirable object or name
 * @returns the object if successful or raises a (catchable) LookupError exception on failure, i.e., if
 * the function times out
 **/
Object WaitForObject(const std::string& object_or_name);

/**
 * @brief WaitForObject waits until the objectOrName object is accessible (i.e., it exists and is visible and enabled).
 * The function waits for the time defined by the optional timeoutMSec parameter is used, that many milliseconds. This
 * function is useful if you want to synchronize your script execution.
 * @param _objectOrName - desirable object or name
 * @param timeoutMSec - timeout in miliseconds
 * @returns the object if successful or raises a (catchable) LookupError exception on failure, i.e., if
 * the function times out.
 **/
Object WaitForObject(const std::string& object_or_name, int timeout_msec);

/**
 * @brief TapObject performs a touch tap at the position specified by screenPoint.
 * Position are in screen global coordinates.
 **/
void TapObject(const ScreenPoint& screen_point);

/**
 * @brief TapObject performs a touch tap at the center of the rectangle specified by screenRectangle.
 * Position are in screen global coordinates.
 **/
void TapObject(const ScreenRectangle& screen_rectangle);
}  // namespace API
}  // namespace squish

#endif  // SQUISH_H_
