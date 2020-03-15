#include "utils/squish_types_converter.h"

namespace common {
namespace squish {

std::string CollectionModeToStr(const common::squish::CollectionMode& mode) {
  // Transform enum common::squish::CollectionMode to string
  std::string collection_mode_str;
  switch (mode) {
    case common::squish::CollectionMode::NONE:
      collection_mode_str = "none";
      break;
    case common::squish::CollectionMode::ANY:
      collection_mode_str = "any";
      break;
    case common::squish::CollectionMode::DAY:
      collection_mode_str = "day_mode";
      break;
    case common::squish::CollectionMode::NIGHT:
      collection_mode_str = "night_mode";
      break;
  }
  return collection_mode_str;
}

}  // namespace squish
}  // namespace common
