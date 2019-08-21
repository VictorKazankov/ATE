#ifndef ATE_SERVER_INTERACTION_INTERACTION_H_
#define ATE_SERVER_INTERACTION_INTERACTION_H_

namespace interaction {
/**
 * @brief Interface class for interaction with LVDS board
 **/
class Interaction {
 public:
  virtual ~Interaction() = default;

  /**
   * @brief Taps screen (press + release) at x,y coordinates
   * @param x x coordinate
   * @param y y coordinate
   **/
  virtual void Tap(const int x, const int y) const = 0;

  /**
   * @brief performs a touch-based drag operation It initiates a touch drag
   * starting at position x, y and drag by dx pixels horizontally and by dy pixels vertically.
   * @param x starting horizontal coordinate
   * @param y starting vertical coordinate
   * @param dx pixels to be dragged vertically
   * @param dy pixels to be dragged horizontally
   **/
  virtual void TouchAndDrag(const int x, const int y, const int dx, const int dy) const = 0;

  /**
   * @brief Presses screen at x,y coordinates
   * @param x x coordinate
   * @param y y coordinate
   **/
  virtual void Press(const int x, const int y) const = 0;

  /**
   * @brief Releases screen at x,y coordinates
   * @param x x coordinate
   * @param y y coordinate
   **/
  virtual void Release(const int x, const int y) const = 0;

  /**
   * @brief Drags mouse pointer to x,y coordinates
   * @param x x coordinate
   * @param y y coordinate
   **/
  virtual void Drag(const int x, const int y) const = 0;
};
}  // namespace interaction

#endif  // ATE_SERVER_INTERACTION_INTERACTION_H_
