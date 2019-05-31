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
};
}  // namespace interaction

#endif  // ATE_SERVER_INTERACTION_DUMMY_DUMMY_INTERACTION_H_
