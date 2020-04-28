#ifndef ATE_CLIENT_CLIENT_CLI_API_MAPPER_H_
#define ATE_CLIENT_CLIENT_CLI_API_MAPPER_H_

#include "application_context.h"

namespace ATE {

// clang-format off

public ref class API {
 public:
  /**  <summary>
        This method causes ATE client to attach to the application called
        <paramref name="aut_name"/> and returns a handle to its application
        context.
      </summary>
      <param name="aut_name">Name of an application to attach to (temporarily ignored by ATE).</param>
      <returns>Handle to attached application context.</returns>
      <exception cref="System::Exception">Thrown if cannot connect to ATE server or internal error has occurred.</exception>
   **/
  static ApplicationContext^ AttachToApplication(System::String^ aut_name);
};

// clang-format on

}  // namespace ATE

#endif  // ATE_CLIENT_CLIENT_CLI_API_MAPPER_H_
