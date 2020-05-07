#ifndef ATE_CLIENT_CLIENT_CLI_API_FORWARDER_H_
#define ATE_CLIENT_CLIENT_CLI_API_FORWARDER_H_

#include <exception>

#include "converters_from_cli.h"
#include "converters_to_cli.h"

namespace ATE {
namespace impl_detail {

/**
 * @brief Invoke API which returns some value with converting all arguments from CLI to
 *        native types before call and converting result back into CLI type
 * @param f API to call
 * @param args Parameter pack represented as CLI types
 * @tparam F Callable type of API
 * @tparam Args Parameter pack type
 * @return API call result converted to CLI type
 * @throw Handle to CLI System::Exception object, converted from original native exception
 **/
template <typename F, typename... Args>
auto InvokeRet(F f, Args... args) try {
  return ToCli(f(FromCli(args)...));
} catch (const std::exception& e) {
  throw Rethrow(e);
}

/**
 * @brief Invoke API which returns void with converting all arguments from CLI to native
 *        types before call
 * @param f API to call
 * @param args Parameter pack represented as CLI types
 * @tparam F Callable type of API
 * @tparam Args Parameter pack type
 * @throw Handle to CLI System::Exception object, converted from original native exception
 **/
template <typename F, typename... Args>
void InvokeVoid(F f, Args... args) try {
  f(FromCli(args)...);
} catch (const std::exception& e) {
  throw Rethrow(e);
}

/**
 * @brief Forwarding functor for WaitForObject() API. Calls the appropriate ::API::WaitForObject()
 *        based on argument pack resolution. This function object can be passed to InvokeRet() template
 *        without taking care of WaitForObject() overloads.
 * @param args Parameter pack
 * @return Selected WaitForObject() call result
 * @throw All exceptions that can throw selected WaitForObject() overload.
 **/
const auto kWaitForObjectForwarder = [](auto... args) { return ::API::WaitForObject(args...); };

/**
 * @brief Forwarding functor for TapObject() API. Calls the appropriate ::API::TapObject()
 *        based on argument pack resolution. This function object can be passed to InvokeVoid() template
 *        without taking care of TapObject() overloads.
 * @param args Parameter pack
 * @return Selected WaitForObject() call result
 * @throw All exceptions that can throw selected WaitForObject() overload.
 **/
const auto kTapObjectForwarder = [](auto... args) { ::API::TapObject(args...); };

}  // namespace impl_detail
}  // namespace ATE

#endif  // ATE_CLIENT_CLIENT_CLI_API_MAPPER_H_
