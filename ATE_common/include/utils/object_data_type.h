#ifndef VDP_ATE_ATE_COMMON_INCLUDE_UTILS_OBJECT_DATA_TYPE_H_
#define VDP_ATE_ATE_COMMON_INCLUDE_UTILS_OBJECT_DATA_TYPE_H_

#include <string>

#include "geometry_types.h"
#include "squish_types.h"

namespace common {

/**
 * @brief An object's data identity structure
 * Needed to represent data from a database to an squish::Object
 */
struct ObjectDataIdentity {
  std::string name;
  std::string sync_version;
  std::string build_version;
  squish::CollectionMode mode{squish::CollectionMode::kNone};
  std::string parent_screen;
};

/**
 * @brief An DTO for image entry from DB
 * Needed to represent data from a database to an squish::Object
 */
struct ObjectData : public ObjectDataIdentity {
  Point center;
  Point top_left;
  Point bottom_right;
  int width;
  int height;
  int parent_width;
  int parent_height;
};

}  // namespace common

#endif  // VDP_ATE_ATE_COMMON_INCLUDE_UTILS_OBJECT_DATA_TYPE_H_
