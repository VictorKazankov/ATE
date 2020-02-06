#ifndef ATE_SERVER_INTERACTION_SPI_SPI_INTERACTION_H_
#define ATE_SERVER_INTERACTION_SPI_SPI_INTERACTION_H_

#include <string>
#include <vector>

#include "interaction/interaction.h"
#include "utils/defines.h"

namespace interaction {

/**
 * @class SpiInteraction
 * @brief Class provides ability to send touch events on TDK via spi interface
 */
class SpiInteraction : public Interaction {
 public:
  SpiInteraction(const std::string& device_address, defines::DisplayType /*display_type*/, int screen_width,
                 int screen_height);
  ~SpiInteraction();

  // Disable copy and move
  SpiInteraction(const SpiInteraction&) = delete;
  SpiInteraction(SpiInteraction&&) = delete;
  SpiInteraction& operator=(const SpiInteraction&) = delete;
  SpiInteraction& operator=(SpiInteraction&&) = delete;

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
  enum MouseEvent { MouseEvent_Press = 'P', MouseEvent_Release = 'R', MouseEvent_Move = 'M' };
  int m_spi_;
  int screen_width_;
  int screen_height_;
  // dummy for unit_test
  const bool need_bus_configuring_ = true;

  void SendEvent(SpiInteraction::MouseEvent evt, int x, int y) const;
  void SendPacket(std::vector<char>& data) const;
};

}  // namespace interaction

#endif  // ATE_SERVER_INTERACTION_SPI_SPI_INTERACTION_H_
