#include "interaction/VDP/https_client.h"

#include <sstream>

#include <boost/asio/buffer.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/write.hpp>

#include "logger/logger.h"

namespace interaction {

namespace {
const std::chrono::milliseconds kReconnectTimeout{100};
}

using boost::asio::ip::tcp;

HttpsClient::HttpsClient(boost::asio::io_context& io_context, const std::string& host, const std::string& port)
    : socket_(io_context, ssl_context_), resolver_(io_context), host_(host), port_(port) {
  ssl_context_.set_default_verify_paths();
  socket_.set_verify_mode(boost::asio::ssl::verify_none);
  Start();
}

void HttpsClient::Start() {
  auto endpoint = resolver_.resolve(host_, port_, error_);
  if (error_) {
    logger::critical("[https client] Can't resolve {}:{} , {}", host_, port_, error_.message());
    return;
  }

  boost::asio::connect(socket_.lowest_layer(), endpoint, error_);

  if (error_) {
    logger::critical("[https client] Can't connect {}:{}, {}", host_, port_, error_.message());
    return;
  }

  socket_.handshake(boost::asio::ssl::stream_base::client, error_);
  if (error_) {
    logger::critical("[https client] Handshake failed {}", host_, port_, error_.message());
    return;
  }
}

void HttpsClient::Stop() {
  logger::info("[https client] Stopping client");
  socket_.lowest_layer().close(error_);

  if (error_) {
    logger::error("[https client] Close of socket failed: {}", error_.message());
  }
}

std::string HttpsClient::PreparePackage(const std::string& command) const {
  std::ostringstream request_stream;
  request_stream << "POST /ontouchevent\r\n";
  request_stream << "Content-Type: application/x-www-form-urlencoded \r\n";
  request_stream << "Content-Length: " << command.length() << "\r\n";
  request_stream << "Accept: text/plain\r\n";
  request_stream << "Connection: keep-alive\r\n\r\n";
  request_stream << command;

  return request_stream.str();
}

void HttpsClient::Send(const std::string& command) {
  auto request = PreparePackage(command);
  std::size_t sent_bytes =
      boost::asio::write(socket_, boost::asio::const_buffer(request.c_str(), request.length()), error_);

  if (error_) {
    logger::error("[https client] write: {}", error_.message());
    return;
  }

  logger::info("[https client] Sent {} bytes to {}:{}", sent_bytes, host_, port_);
}

}  // namespace interaction
