#ifndef ATE_CLIENT_CLIENT_CLI_APPLICATION_CONTEXT_H_
#define ATE_CLIENT_CLIENT_CLI_APPLICATION_CONTEXT_H_

#include "converters.h"
#include "squish/application_context.h"

namespace ATE {

// clang-format off

public ref class ApplicationContext {
 public:
  /** <summary>
        This method detaches a previously attached application.
      </summary>
  **/
  void Detach() { native_ref_.Detach(); }

  /** <summary>
        Every application context is connected to an ATE server. This
        read-only property holds the host name or TCP/IP address of
        the VDP board where the ATE server is running and which this
        application context is connected to.
      </summary>
  **/
  property System::String^ Host {
    System::String^ get() { return impl_detail::ToSystemString(native_ref_.host()); }
  }

  /** <summary>
        Every application context is connected to an ATE server. This
        read-only property holds TCP port number of the ATE server
        which this application context is connected to.
      </summary>
  **/
  property System::String^ Port {
    System::String^ get() { return impl_detail::ToSystemString(native_ref_.port()); }
  }

 internal:
  ApplicationContext(squish::ApplicationContext& native_ref) : native_ref_{native_ref} {}

 private:
  squish::ApplicationContext& native_ref_;
};

// clang-format on

}  // namespace ATE

#endif  // ATE_CLIENT_CLIENT_CLI_APPLICATION_CONTEXT_H_
