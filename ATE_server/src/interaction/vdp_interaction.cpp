#include "interaction/vdp_interaction.h"

#include "boost/asio.hpp"
#include "boost/asio/ssl.hpp"

namespace interaction {

VDPInteraction::VDPInteraction() {
  boost::asio::io_context io_context;
  boost::asio::ip::tcp::resolver resolver(io_context);

  boost::asio::ip::basic_resolver_results<boost::asio::ip::tcp> endpoints =
      resolver.resolve("172.31.239.130", "8444");  // TODO remove hardcode

  boost::asio::ssl::context ctx(boost::asio::ssl::context::sslv23);
  ctx.set_default_verify_paths();

  vdp_client_ = std::make_unique<HttpsClient>(io_context, ctx, endpoints);

  io_context.run();
}

void VDPInteraction::Tap(const int /*x*/, const int /*y*/) const {}

void VDPInteraction::Press(const int /*x*/, const int /*y*/) const {}

void VDPInteraction::Release(const int /*x*/, const int /*y*/) const {}

void VDPInteraction::Drag(const int /*x*/, const int /*y*/) const {}

}  // namespace interaction
