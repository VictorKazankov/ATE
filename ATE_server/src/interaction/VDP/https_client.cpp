#include "interaction/VDP/https_client.h"

#include <sstream>

#include <boost/asio/buffer.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/write.hpp>

#include "utils/logger.h"

namespace interaction {

namespace {
const std::chrono::milliseconds kReconnectTimeout{100};
}

using boost::asio::ip::tcp;

HttpsClient::HttpsClient(boost::asio::io_context& io_context, const std::string& host, const std::string& port)
    : socket_(io_context, ssl_context_), resolver_(io_context), timer_(io_context), host_(host), port_(port) {
  ssl_context_.set_default_verify_paths();
  socket_.set_verify_mode(boost::asio::ssl::verify_none);
}

void HttpsClient::Start() { Resolve(); }

void HttpsClient::Stop() {
  timer_.cancel();

  auto handler = [self = shared_from_this()](const boost::system::error_code& error) {
    if (error) {
      logger::error("[interaction] Shutdown error: {} ({})", error.message(), error);
    }
  };

  socket_.async_shutdown(handler);
}

void HttpsClient::Resolve() {
  auto handler = [self = shared_from_this()](auto&&... params) {
    self->OnResolve(std::forward<decltype(params)>(params)...);
  };

  resolver_.async_resolve(host_, port_, tcp::resolver::numeric_service, handler);
}

void HttpsClient::OnResolve(const boost::system::error_code& error, tcp::resolver::results_type results) {
  if (error) {
    logger::error("[interaction] Cannot resolve: {} ({})", error.message(), error);
    return;
  }

  auto handler = [self = shared_from_this()](auto&&... params) {
    self->OnConnect(std::forward<decltype(params)>(params)...);
  };

  boost::asio::async_connect(socket_.lowest_layer(), results.begin(), results.end(), handler);
}

void HttpsClient::OnConnect(const boost::system::error_code& error, tcp::resolver::results_type::iterator) {
  if (!error) {
    auto handler = [self = shared_from_this()](auto&&... params) {
      self->OnHandshake(std::forward<decltype(params)>(params)...);
    };

    socket_.async_handshake(boost::asio::ssl::stream_base::client, handler);
  } else {
    logger::error("[interaction] Connect failed: {} ({})", error.message(), error);
    Reconnect();
  }
}

void HttpsClient::Reconnect() {
  if (!reconnect_attempts) return;

  --reconnect_attempts;

  timer_.expires_after(kReconnectTimeout);
  auto handler = [self = shared_from_this()](const auto& error) { self->OnReconnect(error); };
  timer_.async_wait(std::move(handler));
}

void HttpsClient::OnReconnect(const boost::system::error_code& error) {
  if (!error) {
    Resolve();
  } else {
    logger::error("[interaction] Can't recconect: {} ({})", error.message(), error);
  }
}

void HttpsClient::OnHandshake(const boost::system::error_code& error) {
  if (error) {
    logger::error("[interaction] Handshake failed: {} ({})", error.message(), error);
    return;
  }

  reconnect_attempts = 3;

  if (message_queue_.empty()) return;

  logger::warn("[interaction] message_queue not empty!");
  SendImpl();
}  // namespace interaction

void HttpsClient::OnSend(const boost::system::error_code& error, std::size_t) {
  if (error) {
    logger::error("[interaction] Write failed: {} ({})", error.message(), error);

    if (boost::system::errc::operation_canceled != error) Reconnect();
  } else {
    message_queue_.pop();
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

void HttpsClient::SendCommand(const std::string& command) {
  logger::debug("[interaction] Command to send: {}", command);

  message_queue_.emplace(PreparePackage(command));
  SendImpl();
}

void HttpsClient::SendCommand(const std::string& first_command, const std::string& second_command) {
  logger::debug("[interaction] Commands to send. First command: {}. Second command: {}", first_command, second_command);

  message_queue_.emplace(PreparePackage(first_command) + PreparePackage(second_command));
  SendImpl();
}

void HttpsClient::SendImpl() {
  auto handler = [self = shared_from_this()](auto&&... params) {
    self->OnSend(std::forward<decltype(params)>(params)...);
  };

  boost::asio::async_write(socket_,
                           boost::asio::const_buffer(message_queue_.front().c_str(), message_queue_.front().length()),
                           std::move(handler));
}

}  // namespace interaction
