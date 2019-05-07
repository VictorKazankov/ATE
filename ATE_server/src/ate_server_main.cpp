#include <boost/asio/io_context.hpp>

#include "ate.h"

int main() {
  boost::asio::io_context io_context;
  ATE ate(io_context);
  io_context.run();

  return EXIT_SUCCESS;
}
