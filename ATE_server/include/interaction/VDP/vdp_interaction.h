#ifndef ATE_SERVER_INTERACTION_VDP_VDP_INTERACTION_H_
#define ATE_SERVER_INTERACTION_VDP_VDP_INTERACTION_H_

#include <boost/asio/io_context.hpp>

#include "https_client.h"
#include "interaction.h"
#include "utils/defines.h"

namespace interaction {

enum class EventType { PRESS, RELEASE, MOVE };

/**
 * @brief Interface class for interaction with LVDS board
 **/
class VDPInteraction : public Interaction {
 private:
  defines::DisplayType display_type_;

  HttpsClient::HttpsClientShared vdp_client_;

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
  std::string PrepareCommand(const int x, const int y, const EventType event_type) const;
};
}  // namespace interaction

#endif  // ATE_SERVER_INTERACTION_VDP_VDP_INTERACTION_H_
