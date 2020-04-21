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

ATEInteraction::ATEInteraction(std::shared_ptr<boost::asio::io_context> io_context, const std::string& host,
                               const std::string& port)
    : host_(host),
      port_(port),
      io_context_(io_context ? std::move(io_context)
                             : throw std::runtime_error{"ATEInteraction: io_context is nullptr"}),
      socket_(*io_context_),
      resolver_{*io_context_},
      read_stream_(&read_buffer_) {
  Connect();
}

bool ATEInteraction::IsConnectionOpened() const {
  if (socket_.is_open()) {
    boost::system::error_code ec;
    char c;

    bool old = socket_.non_blocking();
    socket_.non_blocking(true);

    socket_.read_some(boost::asio::mutable_buffer(&c, 1), ec);

    boost::system::error_code ec_block;
    socket_.non_blocking(old, ec_block);

    logger::debug("[socket.read_some()] error: {} - {}", ec.value(), ec.message());

    return (!ec || boost::asio::error::would_block == ec);
  }
  return false;
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
