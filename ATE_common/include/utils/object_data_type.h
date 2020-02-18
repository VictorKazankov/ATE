#ifndef VDP_ATE_ATE_COMMON_INCLUDE_UTILS_OBJECT_DATA_TYPE_H_
#define VDP_ATE_ATE_COMMON_INCLUDE_UTILS_OBJECT_DATA_TYPE_H_

#include <string>

#include "geometry_types.h"

namespace common {

/**
 * @brief An object data structure.
 * Needed to represent data from a database to an squish::Object
 */
struct ObjectData {
  Point center;
  Point top_left;
  Point bottom_right;
  int width;
  int height;
  int parent_width;
  int parent_height;
  std::string name;
};

}  // namespace common

#endif  // VDP_ATE_ATE_COMMON_INCLUDE_UTILS_OBJECT_DATA_TYPE_H_
