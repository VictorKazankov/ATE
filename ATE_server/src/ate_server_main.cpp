#include <cstdlib>
#include <exception>

#include <boost/asio/io_context.hpp>
#include <boost/asio/signal_set.hpp>

#include "ate.h"
#include "ate_message_adapter.h"
#include "common.h"
#include "exceptions.h"
#include "logger/logger.h"
#include "transport_adapters_collection.h"
#include "utils/defines.h"
#include "version/version_defines.h"

namespace {

void SetupSignalHandling(boost::asio::io_context& io_context, boost::asio::signal_set& sig_set) {
  sig_set.add(SIGINT);
  sig_set.add(SIGTERM);

  sig_set.async_wait([&io_context](const boost::system::error_code& ec, int signo) {
    io_context.stop();
    if (ec) {
      logger::critical("[signal] Unexpected call of the asio::signal_set::async_wait handler with error code: {} ({})",
                       ec.message(), ec);
    } else {
      logger::info("[signal] Signal {} has been caught", signo);
    }
  });
}
}  // namespace

int main() try {
  constexpr auto config_file = VHAT_SERVER_CONFIG_DIR "/vhat_server.ini";
  common::SetUp(config_file);

  logger::info("[initialization] VHAT server version: {}", version::kStringFull);
  logger::info("[initialization] Config file: {}", config_file);

  boost::asio::io_context io_context;

  ATE ate(io_context);
  AteMessageAdapter ate_message_adapter(ate);
  TransportAdaptersCollection transport_adapters;

  transport_adapters.InitTcpConnectionManager(io_context,
                                    common::Config().GetInt(defines::kInteraction, defines::kAteListenerPort, 0));
  transport_adapters.Run();

  boost::asio::signal_set sig_set{io_context};
  SetupSignalHandling(io_context, sig_set);

  io_context.run();

  return EXIT_SUCCESS;
} catch (const boost::system::system_error& boost_error) {
  logger::critical("boost system error: {} ({})", boost_error.what(), boost_error.code());
  return EXIT_FAILURE;
} catch (const std::system_error& system_error) {
  logger::critical("std system error: {} ({})", system_error.what(), system_error.code());
  return EXIT_FAILURE;
} catch (const std::exception& exception) {
  logger::critical("fatal error: {}", exception.what());
  return EXIT_FAILURE;
} catch (...) {  // We cannot assume anything what third party libs might throw
  logger::critical("fatal: unknown error");
  return EXIT_FAILURE;
}
