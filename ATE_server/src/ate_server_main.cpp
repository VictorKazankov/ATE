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
auto& GetMainIoContext() {
  static boost::asio::io_context io_context;
  return io_context;
}
}  // namespace

int main() try {
  constexpr auto config_file = ATE_SERVER_CONFIG_DIR "/ate_server.ini";
  common::SetUp(config_file);

  logger::info("[initialization] ATE server version: {}", version::kStringFull);
  logger::info("[initialization] Config file: {}", config_file);

  ATE ate;
  AteMessageAdapter ate_message_adapter(ate);
  TransportAdaptersCollection transport_adapters(ate_message_adapter, GetMainIoContext());

  transport_adapters.InitTcpConnectionManager(
      common::Config().GetInt(defines::kInteraction, defines::kAteListenerPort, 0));
  transport_adapters.InitDbusConnectionManager(defines::kDBusInterface);
  transport_adapters.InitSigConnectionManager();
  transport_adapters.Run();

  GetMainIoContext().run();

  transport_adapters.Stop();

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
