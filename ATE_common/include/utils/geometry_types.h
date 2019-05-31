#ifndef ATE_COMMON_UTILS_GEOMETRY_TYPES_H_
#define ATE_COMMON_UTILS_GEOMETRY_TYPES_H_

namespace common {

/**
 * @brief Class for 2D points specified by its coordinates `x` and `y`.
 */
struct Point {
  int x = 0;
  int y = 0;

  constexpr Point() noexcept = default;

  Point(int nx, int ny) noexcept;

  /**
   * @brief Converting point of different type to common::Point
   */
  template <typename DifferentPointType>
  Point(const DifferentPointType& rhs) noexcept;

  /**
   * @brief Converting point of different type to common::Point
   */
  template <typename DifferentPointType>
  Point& operator=(const DifferentPointType& rhs) noexcept;

  /**
   * @brief Converting common::Point to point of different type
   */
  template <typename DifferentPointType>
  operator DifferentPointType();
};

/**
 * @brief Class for 2D rectangles specified by its coordinates `x`, `y` and 'width', 'height'.
 * By default each member initialized by zero
 */
struct Rect {
  int x = 0;
  int y = 0;
  int width = 0;
  int height = 0;

  constexpr Rect() noexcept = default;

  Rect(int nx, int ny, int nwidth, int nheight) noexcept;

  /**
   * @brief Converting rectangle of different type to common::Rect
   */
  template <typename DifferentRectType>
  Rect(const DifferentRectType& rhs) noexcept;

  /**
   * @brief Converting rectangle of different type to common::Rect
   */
  template <typename DifferentRectType>
  Rect& operator=(const DifferentRectType& rhs) noexcept;

  /**
   * @brief Converting common::Rect to rectangle of different type
   */
  template <typename DifferentRectType>
  operator DifferentRectType();

/**
   * @brief Gets center of the rectangle
   */
  Point Center() const;
};

template <typename DifferentPointType>
Point::Point(const DifferentPointType& rhs) noexcept : x{rhs.x}, y{rhs.y} {}

template <typename DifferentPointType>
Point& Point::operator=(const DifferentPointType& rhs) noexcept {
  x = rhs.x;
  y = rhs.y;

  return *this;
}

template <typename DifferentPointType>
Point::operator DifferentPointType() {
  DifferentPointType lhs;

  lhs.x = x;
  lhs.y = y;

  return lhs;
}

template <typename DifferentRectType>
Rect::Rect(const DifferentRectType& rhs) noexcept : x{rhs.x}, y{rhs.y}, width{rhs.width}, height{rhs.height} {}

template <typename DifferentRectType>
Rect& Rect::operator=(const DifferentRectType& rhs) noexcept {
  x = rhs.x;
  y = rhs.y;
  width = rhs.width;
  height = rhs.height;

  return *this;
}

template <typename DifferentRectType>
Rect::operator DifferentRectType() {
  DifferentRectType lhs;

  lhs.x = x;
  lhs.y = y;
  lhs.width = width;
  lhs.height = height;

  return lhs;
}

}  // namespace common

#endif  // ATE_COMMON_UTILS_GEOMETRY_TYPES_H_
