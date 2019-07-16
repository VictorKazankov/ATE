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
  SpiInteraction(const std::string& device_address, defines::DisplayType /*display_type*/);
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

 private:
  enum MouseEvent { MouseEvent_Press = 'P', MouseEvent_Release = 'R', MouseEvent_Move = 'M' };
  int m_spi_;
  // dummy for unit_test
  const bool need_bus_configuring_ = true;

  void SendEvent(SpiInteraction::MouseEvent evt, int x, int y) const;
  void SendPacket(std::vector<char>& data) const;
};

}  // namespace interaction

#endif  // ATE_SERVER_INTERACTION_SPI_SPI_INTERACTION_H_
