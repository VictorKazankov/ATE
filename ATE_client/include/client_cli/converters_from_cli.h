#ifndef ATE_CLIENT_CLIENT_CLI_CONVERTERS_FROM_CLI_H_
#define ATE_CLIENT_CLIENT_CLI_CONVERTERS_FROM_CLI_H_

#include <string>

#include "object.h"
#include "squish/squish_types.h"
#include "squish/wildcard.h"
#include "utils/geometry_types.h"
#include "wildcard.h"

namespace ATE {
namespace impl_detail {

// clang-format off

/**
 * @brief Convert CLI System::String into native std::string
 * @param cli_str Handle to System::String object
 * @return copy of cli_string represented as std::string in native memory
 **/
std::string FromCli(System::String^ cli_str);

/**
 * @brief Convert CLI Object into native squish::Object
 * @param cli_object Handle to Object represented as CLI handle
 * @return copy of cli_object represented as squish::Object in native memory
 **/
squish::Object FromCli(ATE::Object^ cli_object);

/**
 * @brief Convert CLI Wildcard into native squish::Wildcard
 * @param cli_wildcard Handle to Wildcard represented as CLI handle
 * @return copy of cli_wildcard represented as squish::Wildcard in native memory
 **/
squish::Wildcard FromCli(Wildcard^ cli_wildcard);

/**
 * @brief Convert CLI ScreenPoint into native common::Point
 * @param cli_point Point represented as CLI ScreenPoint value
 * @return copy of cli_point represented as common::Point type in native memory
 **/
common::Point FromCli(ATE::ScreenPoint cli_point);

/**
 * @brief Convert CLI ScreenRectangle into native common::Rect
 * @param cli_rect Rectangle represented as CLI ScreenRectangle handle
 * @return copy of cli_rect represented as common::Rect type in native memory
 **/
common::Rect FromCli(ATE::ScreenRectangle^ cli_rect);

// clang-format on

}  // namespace impl_detail
}  // namespace ATE

#endif  // ATE_CLIENT_CLIENT_CLI_CONVERTERS_FROM_CLI_H_
