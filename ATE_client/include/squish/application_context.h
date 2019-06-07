#ifndef SQUISH_APPLICATION_CONTEXT_H_
#define SQUISH_APPLICATION_CONTEXT_H_

#include <memory>
#include <string>

#include <boost/asio/io_context.hpp>

#include "ate_interaction.h"
#include "squish/squish_types.h"

namespace squish {

struct ApplicationContext {
  std::string host{};
  std::string port{};

 private:
  std::unique_ptr<interaction::ATEInteraction> ate_interaction;

 public:
  ApplicationContext() = default;
  ~ApplicationContext() = default;
  ApplicationContext(const squish::ApplicationContext& rhs);

  /**
   * @brief Function establishes connection to ATE server
   * @param io_context context for communications
   **/
  void Attach(boost::asio::io_context& io_context);

  /**
   * @brief The function sends commands to the target
   * @param requested command
   * @returns result object
   **/
  Object SendCommand(interaction::Method method, const std::string& command);

  /**
   * @brief Function allows to know if connection was established
   * @returns true if connected, otherwise - false
   **/
  bool IsRunning() const;

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

}  // namespace squish
#endif  // SQUISH_APPLICATION_CONTEXT_H_
