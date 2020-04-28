#ifndef ATE_CLIENT_CLIENT_CLI_CONVERTERS_H_
#define ATE_CLIENT_CLIENT_CLI_CONVERTERS_H_

#include <string>

namespace ATE {
namespace impl_detail {

// clang-format off

/**
 * @brief Convert System::String into std::string
 * @param s Handle to System::String object
 * @return copy of s represented as std::string
 **/
std::string ToStdString(System::String^ s);

/**
 * @brief Convert std::string into System::String
 * @param s String represented as std::string
 * @return Handle to System::String object which holds copy of s
 **/
System::String^ ToSystemString(const std::string& s);

// clang-format on

}  // namespace impl_detail
}  // namespace ATE

#endif  // ATE_CLIENT_CLIENT_CLI_CONVERTERS_H_
