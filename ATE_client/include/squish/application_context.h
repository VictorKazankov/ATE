#ifndef SQUISH_APPLICATION_CONTEXT_H_
#define SQUISH_APPLICATION_CONTEXT_H_

#include <memory>
#include <string>

#include "interaction.h"
#include "squish/squish_types.h"

namespace squish {

class ApplicationContext {
 public:
  ApplicationContext() = default;
  ~ApplicationContext() = default;
  ApplicationContext(const ApplicationContext&) = delete;
  ApplicationContext& operator=(const ApplicationContext&) = delete;

  /**
   * @brief Function establishes connection to ATE server
   * @param ate_interaction For communication
   **/
  void Attach(const std::shared_ptr<interaction::Interaction>& ate_interaction);

  /**
   * @brief The function sends commands to the target
   * @param command Command to be send
   * @returns The response from server-side
   **/
  std::string SendCommand(const std::string& command);

  /**
   * @brief Function allows to know if connection was established
   * @returns True if connected, otherwise - false
   **/
  bool IsRunning() const;

  /**
   * @brief This function detaches from a previously started (or attached) application.
   *        If the respective application has been started with startApplication Squish will attempt to close it, and
   *        eventually (if still running) forcefully close it.
   *        If the respective application has been attached to with attachToApplication Squish will leave it running but
   *        detach from it, thus leaving the application running. The application can then be attached to with
   *        attachToApplication again. After detaching from an application one should not access any of its objects
   *        anymore. (This would typically be done by using object references which had been retrieved before
   *        detaching.)
   **/
  void Detach();

  /**
   * @brief Function provides host
   * @return The host name
   **/
  std::string host() const;

  /**
   * @brief Function provides port
   * @return The port number in the string representation
   **/
  std::string port() const;

 private:
  std::shared_ptr<interaction::Interaction> ate_interaction_;
};

}  // namespace squish
#endif  // SQUISH_APPLICATION_CONTEXT_H_
