#ifndef ATE_SERVER_INTERACTION_DUMMY_DUMMY_INTERACTION_H_
#define ATE_SERVER_INTERACTION_DUMMY_DUMMY_INTERACTION_H_

#include "interaction/interaction.h"

namespace interaction {

class DummyInteraction : public Interaction {
 public:
  ~DummyInteraction() override = default;

  /**
   * @brief Taps screen (press + release) at x, y coordinates
   * @param x x coordinate
   * @param y y coordinate
   **/
  void Tap(const int x, const int y) const override;

  /**
   * @brief performs a touch-based drag operation It initiates a touch drag
   * starting at position x, y and drag by dx pixels horizontally and by dy pixels vertically.
   * @param x starting horizontal coordinate
   * @param y starting vertical coordinate
   * @param dx pixels to be dragged vertically
   * @param dy pixels to be dragged horizontally
   **/
  void TouchAndDrag(const int x, const int y, const int dx, const int dy) const override;

  /**
   * @brief Presses screen at x, y coordinates
   * @param x x coordinate
   * @param y y coordinate
   */
  void Press(const int x, const int y) const override;

  /**
   * @brief Releases screen at x, y coordinates
   * @param x x coordinate
   * @param y y coordinate
   */
  void Release(const int x, const int y) const override;

  /**
   * @brief Drags mouse pointer to x, y coordinates
   * @param x x coordinate
   * @param y y coordinate
   */
  void Drag(const int x, const int y) const override;

  /**
   * @brief ChangeResolution sets current resolution
   * @param screen_width screen resolution over width
   * @param screen_height screen resolution over height
   */
  void ChangeResolution(int screen_width, int screen_height) override;
};
}  // namespace interaction

#endif  // ATE_SERVER_INTERACTION_DUMMY_DUMMY_INTERACTION_H_
