#ifndef ATE_SERVER_INTERACTION_VDP_VDP_INTERACTION_H_
#define ATE_SERVER_INTERACTION_VDP_VDP_INTERACTION_H_

#include <boost/asio/io_context.hpp>

#include "interaction/interaction.h"
#include "utils/defines.h"

namespace interaction {

enum class EventType { PRESS, RELEASE, MOVE };

/**
 * @brief Interface class for interaction with LVDS board
 **/
class VDPInteraction : public Interaction {
 private:
  boost::asio::io_context& context_;
  std::string host_;
  std::string port_;
  defines::DisplayType display_type_;

 public:
  VDPInteraction(boost::asio::io_context& io_context, const std::string& ip_address, const std::string& port,
                 defines::DisplayType display_type);
  ~VDPInteraction() override;

  /**
   * @brief Taps screen (press + release) at x,y coordinates
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
   * @brief Presses screen at x,y coordinates
   * @param x x coordinate
   * @param y y coordinate
   **/
  void Press(const int x, const int y) const override;

  /**
   * @brief Releases screen at x,y coordinates
   * @param x x coordinate
   * @param y y coordinate
   **/
  void Release(const int x, const int y) const override;

  /**
   * @brief Drags mouse pointer to x,y coordinates
   * @param x x coordinate
   * @param y y coordinate
   **/
  void Drag(const int x, const int y) const override;

  /**
   * @brief ChangeResolution sets current resolution
   * @param screen_width screen resolution over width
   * @param screen_height screen resolution over height
   */
  void ChangeResolution(int screen_width, int screen_height) override;

 private:
  std::string PrepareCommand(const int x, const int y, const EventType event_type) const;
};
}  // namespace interaction

#endif  // ATE_SERVER_INTERACTION_VDP_VDP_INTERACTION_H_
