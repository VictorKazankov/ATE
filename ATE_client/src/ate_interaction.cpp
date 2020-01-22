#include "ate_interaction.h"

#include <chrono>
#include <thread>

#include <boost/asio/connect.hpp>
#include <boost/asio/read_until.hpp>
#include <boost/asio/write.hpp>

#include "logger/logger.h"

namespace interaction {

using namespace std::chrono;

namespace {
const auto kReconnectTimeout{1s};
}  // namespace

ATEInteraction::ATEInteraction(boost::asio::io_context& io_context, const std::string& host, const std::string& port)
    : host_(host), port_(port), socket_(io_context), resolver_(io_context), read_stream_(&read_buffer_) {
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
    reconnect_attempts = kAttempts;
  }
}

void ATEInteraction::Reconnect() {
  logger::error("[interaction] Reconnect attempt left {}", reconnect_attempts);
  --reconnect_attempts;

  std::this_thread::sleep_for(kReconnectTimeout);
  Connect();
}

std::string ATEInteraction::SendCommand(const std::string& command) {
  boost::asio::write(socket_, boost::asio::buffer(command), boost::asio::transfer_all());
  boost::asio::read_until(socket_, read_buffer_, '\n');

  std::string message;
  std::getline(read_stream_, message);
  logger::debug("[interaction] Received message: {}", message);

  return message;
}
}  // namespace interaction
