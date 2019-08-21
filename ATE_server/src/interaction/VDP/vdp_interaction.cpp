#include "interaction/VDP/vdp_interaction.h"
#include "interaction/VDP/https_client.h"
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
    : context_(io_context), host_(ip_address), port_(port), display_type_(display_type) {}

VDPInteraction::~VDPInteraction() {}

std::string VDPInteraction::PrepareCommand(const int x, const int y, const EventType event_type) const {
  std::ostringstream command_str;
  command_str << "messageType=SIMPLE_TOUCH"
              << "&displayType=(" << static_cast<int>(display_type_) << ",)"
              << "&contactId=" << static_cast<int>(ContactId::ONE_FINGER_TOUCH)
              << "&eventType=" << EventTypeToString(event_type) << "&positionX=" << x << "&positionY=" << y;

  return command_str.str();
}

void VDPInteraction::Tap(const int x, const int y) const {
  HttpsClient client(context_, host_, port_);
  client.Send(PrepareCommand(x, y, EventType::PRESS));
  client.Send(PrepareCommand(x, y, EventType::RELEASE));
}

void VDPInteraction::TouchAndDrag(const int, const int, const int, const int) const {
  // Dummy drag for VDP interaction
  assert(false);
}

void VDPInteraction::Press(const int x, const int y) const {
  HttpsClient client(context_, host_, port_);
  client.Send(PrepareCommand(x, y, EventType::PRESS));
}

void VDPInteraction::Release(const int x, const int y) const {
  HttpsClient client(context_, host_, port_);
  client.Send(PrepareCommand(x, y, EventType::RELEASE));
}

void VDPInteraction::Drag(const int x, const int y) const {
  HttpsClient client(context_, host_, port_);
  client.Send(PrepareCommand(x, y, EventType::MOVE));
}

}  // namespace interaction
