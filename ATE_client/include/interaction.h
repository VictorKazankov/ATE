#ifndef INTERACTION_H_
#define INTERACTION_H_

namespace interaction {

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
   * @return True if socket is open otherwise false.
   **/
  virtual bool IsConnectionOpened() const = 0;

  /**
   * @brief Send command to server
   * @param command Specifies string command
   * @return Response string
   */
  virtual std::string SendCommand(const std::string& command) = 0;
};

}  // namespace interaction

#endif  // INTERACTION_H_
