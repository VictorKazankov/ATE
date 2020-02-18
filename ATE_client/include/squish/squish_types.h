#ifndef SQUISH_SQUISH_TYPES_H_
#define SQUISH_SQUISH_TYPES_H_

#include <string>

#include "utils/geometry_types.h"

namespace squish {

struct Object : common::Rect {
  Object() = default;
  Object(int x, int y) : common::Rect{x, y, 0, 0} {};
  Object(int x, int y, int width, int height) : common::Rect{x, y, width, height} {};

  /**
   * @brief Get top left corner coordinates
   * @return Top left corner coordinate
   */
  common::Point TopLeft() { return common::Point{x_top_left, y_top_left}; }

  /**
   * @brief Get bottom right corner coordinates
   * @return Bottom right corner coordinate
   */
  common::Point BottomRight() { return common::Point{x_bottom_right, y_bottom_right}; }

  // Data section
  int x_top_left{-1};
  int y_top_left{-1};
  int x_bottom_right{-1};
  int y_bottom_right{-1};
  int parent_width{-1};
  int parent_height{-1};
  std::string name;
  std::string type;
};

}  // namespace squish

#endif  // SQUISH_SQUISH_TYPES_H_
