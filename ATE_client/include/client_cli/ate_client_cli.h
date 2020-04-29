#ifndef ATE_CLIENT_CLIENT_CLI_API_MAPPER_H_
#define ATE_CLIENT_CLIENT_CLI_API_MAPPER_H_

#include "application_context.h"
#include "error_defines.h"
#include "errors.h"
#include "object.h"

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
      <exception cref="System::Exception">Thrown if internal error has occured.</exception>
   **/
  static ATE::Object^ WaitForObject(ATE::Object^ objectOrName);

  /** <summary>
        waitForObject waits until the <paramref name="objectOrName"/> object is accessible (i.e., it exists and is visible).
        Method waits for the time defined by the waitForObjectTimeout config setting, that many milliseconds.
      </summary>
      <param name="objectOrName">Object or name to wait for.</param>
      <returns>Object if successful</returns>
      <exception cref="LookupError">Thrown on failure, i.e. if the method times out</exception>
      <exception cref="System::Exception">Thrown if internal error has occured.</exception>
   **/
  static ATE::Object^ WaitForObject(System::String^ objectOrName);

  /** <summary>
        waitForObject waits until the <paramref name="objectOrName"/> object is accessible (i.e., it exists and is visible).
        Method waits for the time defined by the <paramref name="timeoutMSec"/>, that many milliseconds.
      </summary>
      <param name="objectOrName">Object or name to wait for.</param>
      <returns>Object if successful</returns>
      <exception cref="LookupError">Thrown on failure, i.e. if the method times out</exception>
      <exception cref="System::Exception">Thrown if internal error has occured.</exception>
   **/
  static ATE::Object^ WaitForObject(ATE::Object^ objectOrName, int timeoutMSec);

  /** <summary>
        waitForObject waits until the <paramref name="objectOrName"/> object is accessible (i.e., it exists and is visible).
        Method waits for the time defined by the <paramref name="timeoutMSec"/>, that many milliseconds.
      </summary>
      <param name="objectOrName">Object or name to wait for.</param>
      <returns>Object if successful</returns>
      <exception cref="LookupError">Thrown on failure, i.e. if the method times out</exception>
      <exception cref="System::Exception">Thrown if internal error has occured.</exception>
   **/
  static ATE::Object^ WaitForObject(System::String^ objectOrName, int timeoutMSec);
};

// clang-format on

}  // namespace ATE

#endif  // ATE_CLIENT_CLIENT_CLI_API_MAPPER_H_
