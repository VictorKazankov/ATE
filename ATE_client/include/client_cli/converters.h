#ifndef ATE_CLIENT_CLIENT_CLI_CONVERTERS_H_
#define ATE_CLIENT_CLIENT_CLI_CONVERTERS_H_

#include <string>

#include "object.h"
#include "squish/squish_types.h"

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

/**
 * @brief Convert ATE::Object into squish::Object
 * @param o Handle to Object represented as ATE::Object
 * @return copy of o represented as squish::Object
 **/
squish::Object ToSquishObject(ATE::Object^ o);

/**
 * @brief Convert squish::Object to ATE::Object
 * @param o Object represented as squish::Object type
 * @return Handle to ATE::Object object which hold copy of o
 **/
ATE::Object^ ToAteObject(const squish::Object& o);

// clang-format on

}  // namespace impl_detail
}  // namespace ATE

#endif  // ATE_CLIENT_CLIENT_CLI_CONVERTERS_H_
