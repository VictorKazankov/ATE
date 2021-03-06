#include "utils/geometry_types.h"

namespace common {

Point::Point(int nx, int ny) noexcept : x{nx}, y{ny} {}

Rect::Rect(int nx, int ny, int nwidth, int nheight) noexcept : x{nx}, y{ny}, width{nwidth}, height{nheight} {}

Point Rect::Center() const { return Point(width / 2 + x, height / 2 + y); }

}  // namespace common
