#include "interaction/VDP/vdp_interaction.h"

#include <sstream>

namespace interaction {

namespace {
enum class ContactId { ONE_FINGER_TOUCH = 0 };

const char* EventTypeToString(EventType e) {
  switch (e) {
    case EventType::PRESS:
      return "PRESS";
    case EventType::RELEASE:
      return "RELEASE";
    case EventType::MOVE:
      return "MOVE";
    default:
      return "UNDEFINED";
  }
}
}  // namespace

VDPInteraction::VDPInteraction(boost::asio::io_context& io_context, const std::string& ip_address,
                               const std::string& port, defines::DisplayType display_type)
    : display_type_(display_type), vdp_client_(HttpsClient::CreateHttpsClient(io_context, ip_address, port)) {
  vdp_client_->Start();
}

VDPInteraction::~VDPInteraction() { vdp_client_->Stop(); }

std::string VDPInteraction::PrepareCommand(const int x, const int y, const EventType event_type) const {
  std::ostringstream command_str;
  command_str << "messageType=SIMPLE_TOUCH"
              << "&displayType=(" << static_cast<int>(display_type_) << ",)"
              << "&contactId=" << static_cast<int>(ContactId::ONE_FINGER_TOUCH)
              << "&eventType=" << EventTypeToString(event_type) << "&positionX=" << x << "&positionY=" << y;

  return command_str.str();
}

void VDPInteraction::Tap(const int x, const int y) const {
  vdp_client_->SendCommand(PrepareCommand(x, y, EventType::PRESS), PrepareCommand(x, y, EventType::RELEASE));
}

void VDPInteraction::Press(const int x, const int y) const {
  vdp_client_->SendCommand(PrepareCommand(x, y, EventType::PRESS));
}

void VDPInteraction::Release(const int x, const int y) const {
  vdp_client_->SendCommand(PrepareCommand(x, y, EventType::RELEASE));
}

void VDPInteraction::Drag(const int x, const int y) const {
  vdp_client_->SendCommand(PrepareCommand(x, y, EventType::MOVE));
}

}  // namespace interaction
