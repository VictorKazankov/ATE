#ifndef INTERACTION_H_
#define INTERACTION_H_

#include "squish/squish_types.h"

namespace interaction {

enum class Method {
  kAttachToApplication,
  kWaitForObject,
  kTapObject,
  kTouchAndDrag,
  kPressAndHold,
  kChangeSyncIconDB,
  kChangeSyncMode,
  kLongPress
};

class Interaction {
 public:
  virtual ~Interaction() = default;

  /**
   * @brief Function provides host
   * Named according to squish API
   * @return host
   **/
  virtual std::string host() const = 0;

  /**
   * @brief Function provides port
   * Named according to squish API
   * @return port
   **/
  virtual std::string port() const = 0;

  /**
   * @brief Function determine whether the socket is open.
   * @return return true if socket is open otherwise false.
   **/
  virtual bool IsConnectionOpened() const = 0;

  /**
   * @brief Send command to server
   * @param method - specifies method
   * @param command - specifies string command
   * @return squish object
   */
  virtual squish::Object SendCommand(Method method, const std::string& command) = 0;
};

}  // namespace interaction

#endif  // INTERACTION_H_
