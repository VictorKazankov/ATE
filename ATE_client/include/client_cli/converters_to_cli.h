#ifndef ATE_CLIENT_CLIENT_CLI_CONVERTERS_TO_CLI_H_
#define ATE_CLIENT_CLIENT_CLI_CONVERTERS_TO_CLI_H_

#include <string>
#include <vector>

#include "application_context.h"
#include "enums.h"
#include "object.h"
#include "squish/application_context.h"
#include "squish/squish_types.h"
#include "utils/squish_types.h"

namespace ATE {
namespace impl_detail {

// clang-format off

/**
 * @brief Convert string into CLI string
 * @param native_str String represented as pointer to const char in native memory
 * @return Handle to System::String CLI object which holds copy of original native_str
 **/
System::String^ ToCli(const char* native_str);

/**
 * @brief Convert string into CLI string
 * @param native_str String represented as std::string in native memory
 * @return Handle to System::String CLI object which holds copy of original native_str
 **/
System::String^ ToCli(const std::string& native_str);

/**
 * @brief Convert squish::ApplicationContext into its CLI counterpart
 * @param app_context reference to ApplicationContext in native memory
 * @return Handle to ApplicationContext CLI object with reference to app_context
 **/
ApplicationContext^ ToCli(squish::ApplicationContext& app_context);

/**
 * @brief Convert CollectionMode into its CLI counterpart
 * @param mode value to convert
 * @return value of CLI enum type
 **/
CollectionMode ToCli(common::squish::CollectionMode mode);

/**
 * @brief Convert squish::Object  into its CLI counterpart
 * @param object reference to squish::Object in native memory
 * @return Handle to CLI ATE::Object which is copy of original object
 **/
ATE::Object^ ToCli(const squish::Object& object);

/**
 * @brief Convert Object list into its CLI counterpart
 * @param objects reference to vector of squish::Objects in native memory
 * @return Handle to CLI List which holds copies of original objects
 **/
System::Collections::Generic::List<ATE::Object^>^ ToCli(const std::vector<squish::Object>& objects);

/**
 * @brief Convert string list into its CLI counterpart
 * @param objects reference to vector of std::strings in native memory
 * @return Handle to CLI List which holds copies of original strings
 **/
System::Collections::Generic::List<System::String^>^ ToCli(const std::vector<std::string>& strings);

// clang-format on

/**
 * @brief Dummy converter for types that are compatible between CLI and
 *        native worlds and which simply does nothing. This template
 *        converter will be chosen by C++ compiler, in case there is no
 *        better non-template overload for x value argument. Example of
 *        type that cannot be converted is plain int
 * @param x Value to be converted
 * @tparam T type of x
 * @return same value as x
 **/
template <typename T>
auto ToCli(T x) {
  return x;
}

}  // namespace impl_detail
}  // namespace ATE

#endif  // ATE_CLIENT_CLIENT_CLI_CONVERTERS_TO_CLI_H_
