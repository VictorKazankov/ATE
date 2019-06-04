#include "ate_interaction.h"

#include <unistd.h>
#include <chrono>
#include <thread>

#include <boost/asio/connect.hpp>
#include <boost/asio/read_until.hpp>
#include <boost/asio/write.hpp>

#include "error_defines.h"
#include "logger/logger.h"
#include "message_factory/json_defines.h"
#include "message_factory/json_messages.h"
#include "message_factory/json_utils.h"

namespace interaction {

namespace {
const std::chrono::seconds kReconnectTimeout{1};

/**
 * @brief Verify area if area equel zero throw LookupError
 **/
bool CheckIsAreaEmpty(const squish::Object& obj) { return ((0 != obj.height) && (0 != obj.width)); }

bool CheckJsonStructure(const std::string& message, Json::Value& value) {
  if (!common::jmsg::ParseJson(message, value)) {
    return false;
  }
  if (!(common::jmsg::CheckHeader(value) || common::jmsg::CheckHeaderType(value))) {
    return false;
  }
  return true;
}

squish::Object ParseMessage(const std::string& method, const Json::Value& schema) {
  squish::Object object;

  if (schema.isMember(common::jmsg::kError)) throw squish::LookupError{};

  if (method == common::jmsg::kWaitForObject && common::jmsg::CheckWaitForObjectResponse(schema)) {
    logger::debug("[parse message] waitForObject response");

    auto& result = schema[common::jmsg::kResult];

    object.x = result[common::jmsg::kAbscissa].asInt();
    object.y = result[common::jmsg::kOrdinate].asInt();
    object.width = result[common::jmsg::kWidth].asInt();
    object.height = result[common::jmsg::kHight].asInt();

    if (!CheckIsAreaEmpty(object)) {
      throw squish::LookupError{};
    }
  }
  if (method == common::jmsg::kTapObject) {
    logger::info("[parse message] tapObject response");
    // TODO: prepare handling logic
  }
  if (method == common::jmsg::kAttachToApplication) {
    logger::info("[parse message] attachToApplication response");
    // TODO: prepare handling logic
  }
  return object;
}
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

squish::Object ATEInteraction::SendCommand(const std::string& command) {
  boost::asio::write(socket_, boost::asio::buffer(command), boost::asio::transfer_all());
  boost::asio::read_until(socket_, read_buffer_, '\n');

  std::string message;
  std::getline(read_stream_, message);
  logger::debug("[interaction] Received message: {}", message);

  Json::Value json_structure;

  if (!CheckJsonStructure(message, json_structure)) {
    return squish::Object{};
  }

  const auto& method = json_structure[common::jmsg::kMethod].asString();

  return ParseMessage(method, json_structure);
}
}  // namespace interaction
