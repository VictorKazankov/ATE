#include "utils/squish_types_converter.h"

#include <stdexcept>

namespace common {
namespace squish {

namespace {
const std::string kNoneEnumValuesString = "NONE";
const std::string kAnyEnumValuesString = "ANY";
const std::string kDayEnumValuesString = "DAY";
const std::string kNightEnumValuesString = "NIGHT";
}  // namespace

std::string CollectionModeToStr(const common::squish::CollectionMode& mode) {
  std::string collection_mode_str;
  switch (mode) {
    case common::squish::CollectionMode::kNone:
      collection_mode_str = kNoneEnumValuesString;
      break;
    case common::squish::CollectionMode::kAny:
      collection_mode_str = kAnyEnumValuesString;
      break;
    case common::squish::CollectionMode::kDay:
      collection_mode_str = kDayEnumValuesString;
      break;
    case common::squish::CollectionMode::kNight:
      collection_mode_str = kNightEnumValuesString;
      break;
  }
  return collection_mode_str;
}

CollectionMode StrToCollectionMode(const std::string& mode) {
  if (mode == kNoneEnumValuesString) return common::squish::CollectionMode::kNone;
  if (mode == kAnyEnumValuesString) return common::squish::CollectionMode::kAny;
  if (mode == kDayEnumValuesString) return common::squish::CollectionMode::kDay;
  if (mode == kNightEnumValuesString) return common::squish::CollectionMode::kNight;

  throw std::runtime_error("Undefided CollectionMode value");
}  // namespace squish

}  // namespace squish
}  // namespace common
