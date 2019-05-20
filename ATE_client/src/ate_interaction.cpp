#include <unistd.h>
#include <chrono>

#include "ate_interaction.h"

#include <boost/asio/connect.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/write.hpp>

#include "logger/logger.h"

namespace interaction {

namespace {
const std::chrono::seconds kReconnectTimeout{1};
}

ATEInteraction::ATEInteraction(boost::asio::io_context& io_context, const std::string& host, const std::string& port)
    : host_(host), port_(port), socket_(io_context), resolver_(io_context) {
  Connect();
}

void ATEInteraction::Connect() {
  auto endpoints = resolver_.resolve(host_, port_);
  auto endpoint_iterator = endpoints.begin();

  boost::system::error_code error = boost::asio::error::host_not_found;

  while (error && endpoint_iterator != endpoints.end()) {
    socket_.close();
    socket_.connect(*endpoint_iterator++, error);
  }

  if (error) {
    logger::error("[interaction] Cannot connect: {} ({})", error.message(), error);

    if (reconnect_attempts) {
      Reconnect();
    } else {
      throw std::runtime_error("Connection attempts to " + host_ + ":" + port_ + " was exhausted");
    }

  } else {
    reconnect_attempts = 3;
  }
}

void ATEInteraction::Reconnect() {
  logger::error("[interaction] Reconnect attempt left {}", reconnect_attempts);
  --reconnect_attempts;

  sleep(kReconnectTimeout.count());
  Connect();
}

squish::Object ATEInteraction::SendCommand(const std::string& command) {
  boost::system::error_code error;
  boost::asio::write(socket_, boost::asio::buffer(command), boost::asio::transfer_all(), error);

  if (error) {
    logger::error("[interaction] Write failed: {} ({})", error.message(), error);
  }

  socket_.read_some(boost::asio::buffer(read_buffer_), error);

  logger::debug("[interaction] Received message: {}", read_buffer_);

  return squish::Object();  // TODO convert read_buffer_ convert to an object
}
}  // namespace interaction
