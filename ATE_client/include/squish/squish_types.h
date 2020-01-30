#ifndef SQUISH_SQUISH_TYPES_H_
#define SQUISH_SQUISH_TYPES_H_

#include <string>
#include "utils/geometry_types.h"

namespace squish {

struct Object : common::Rect {
  Object(){};
  Object(int x, int y) : common::Rect{x, y, 0, 0} {};
  Object(int x, int y, int width, int height) : common::Rect{x, y, width, height} {};

  std::string name;
  std::string type;
};

}  // namespace squish

#endif  // SQUISH_SQUISH_TYPES_H_
