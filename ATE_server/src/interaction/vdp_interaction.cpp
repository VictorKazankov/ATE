#include "interaction/vdp_interaction.h"

#include "boost/asio.hpp"
#include "boost/asio/ssl.hpp"

#include <sstream>

namespace interaction {

namespace {
enum class ContactId { ONE_FINGER_TOUCH = 0 };

const char* EventTypeToString(EventType e) {
  const std::map<EventType, const char*> EventTypeStrings{
      {EventType::PRESS, "PRESS"}, {EventType::RELEASE, "RELEASE"}, {EventType::MOVE, "MOVE"}};
  auto it = EventTypeStrings.find(e);
  return it == EventTypeStrings.end() ? "Out of range" : it->second;
}
}  // namespace

VDPInteraction::VDPInteraction(boost::asio::io_context& io_context, const std::string& ip_address,
                               const std::string& port, defines::DisplayType display_type)
    : display_type_(display_type) {
  boost::asio::ip::tcp::resolver resolver(io_context);

  boost::asio::ip::basic_resolver_results<boost::asio::ip::tcp> endpoints = resolver.resolve(ip_address, port);

  boost::asio::ssl::context ctx(boost::asio::ssl::context::sslv23);
  ctx.set_default_verify_paths();

  vdp_client_ = std::make_unique<HttpsClient>(io_context, ctx, endpoints);
}

std::string VDPInteraction::PrepareCommand(const int x, const int y, const EventType event_type) const noexcept {
  std::ostringstream command_str;
  command_str << "messageType=SIMPLE_TOUCH"
              << "&displayType=(" << static_cast<int>(display_type_) << ",)"
              << "&contactId=" << static_cast<int>(ContactId::ONE_FINGER_TOUCH)
              << "&eventType=" << EventTypeToString(event_type) << "&positionX=" << x << "&positionY=" << y;

  return command_str.str();
}

void VDPInteraction::Tap(const int x, const int y) const {
  Press(x, y);
  Release(x, y);
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
