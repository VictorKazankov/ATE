#ifndef SQUISH_SQUISH_TYPES_H_
#define SQUISH_SQUISH_TYPES_H_

#include <string>
#include "utils/geometry_types.h"

namespace squish {

struct Object : common::Rect {
  Object(){};
  std::string name;
  std::string type;
};

}  // namespace squish

#endif  // SQUISH_SQUISH_TYPES_H_
