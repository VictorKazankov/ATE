#include <iostream>

#include <boost/asio/io_context.hpp>
#include <boost/asio/signal_set.hpp>

#include "ate.h"

namespace {
void SetupSignalHandling(boost::asio::io_context& io_context, boost::asio::signal_set& sig_set) {
  sig_set.add(SIGINT);
  sig_set.add(SIGTERM);

  sig_set.async_wait([&io_context](const boost::system::error_code& ec, int signo) {
    io_context.stop();
    if (ec) {
      std::clog << "FATAL: Unexpected call of the asio::signal_set::async_wait handler with error code: "
                << ec.message() << " (" << ec << ")\n";
    } else {
      std::clog << "INFO: Signal " << signo << " has been caught\n";
    }
  });
}
}  // namespace

int main() {
  boost::asio::io_context io_context;

  ATE ate(io_context);

  boost::asio::signal_set sig_set{io_context};
  SetupSignalHandling(io_context, sig_set);

  io_context.run();

  return EXIT_SUCCESS;
}
