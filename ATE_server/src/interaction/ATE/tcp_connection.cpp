#include "interaction/ATE/tcp_connection.h"

#include <boost/asio/buffer.hpp>
#include <boost/asio/read_until.hpp>
#include <boost/asio/write.hpp>

#include "logger/logger.h"

namespace interaction {

TcpConnection::TcpConnection(boost::asio::io_context& service) : socket_(service), read_stream_(&read_buffer_) {}

std::string TcpConnection::Address() const { return address_; }

void TcpConnection::Start() {
  if (!running_) {
    SetAddress();
    running_ = true;
    logger::info("[tcpconnection] Listening to messages from - {}", Address());
    Receive();
  }
}

void TcpConnection::Stop() {
  if (running_) {
    running_ = false;
    logger::info("[tcpconnection] Stopping client - {}", Address());
    boost::system::error_code error;
    socket_.cancel(error);
    if (error) {
      logger::error("[tcpconnection] {}", error.message());
    }
  }
}

void TcpConnection::Send(const std::string& message) {
  if (!running_) return;
  message_queue_.emplace(message);
  SendImpl();
}

void TcpConnection::SendImpl() {
  auto handler = [self = shared_from_this()](auto&&... params) {
    self->OnSend(std::forward<decltype(params)>(params)...);
  };
  logger::info("[tcpconnection] Sending message - {} - to client - {}", message_queue_.front(), Address());
  boost::asio::async_write(socket_,
                           boost::asio::const_buffer(message_queue_.front().data(), message_queue_.front().length()),
                           std::move(handler));
}

void TcpConnection::OnSend(const boost::system::error_code& error, std::size_t bytes_transferred) {
  message_queue_.pop();

  if (error) {
    if (error == boost::asio::error::eof || error == boost::asio::error::connection_reset) {
      logger::info("[tcpconnection] Client - {}  - disconnected", Address());
    } else {
      if (error != boost::system::errc::operation_canceled) {
        logger::error("[tcpconnection] Error on receiving message from - {} - {}", Address(), error.message());
      }
    }
    Stop();
    return;
  }

  logger::debug("[tcpconnection] Sent {} bytes", bytes_transferred);
}

void TcpConnection::Receive() {
  if (!running_) return;

  auto handler = [self = shared_from_this()](auto&&... params) {
    self->OnReceive(std::forward<decltype(params)>(params)...);
  };
  boost::asio::async_read_until(socket_, read_buffer_, '\n', std::move(handler));
}

void TcpConnection::OnReceive(const boost::system::error_code& error, std::size_t bytes_transferred) {
  if (error) {
    if (error == boost::asio::error::eof || error == boost::asio::error::connection_reset) {
      logger::info("[tcpconnection] Client - {} - disconnected", Address());
    } else {
      if (error != boost::system::errc::operation_canceled) {
        logger::error("[tcpconnection] Error on receiving message from - {} - {}", Address(), error.message());
      }
    }

    Stop();
    return;
  }

  if (handler_) {
    std::string message;
    std::getline(read_stream_, message);

    logger::debug("[tcpconnection] Received: {} bytes from - {}", bytes_transferred, Address());
    handler_->OnMessage(shared_from_this(), message);
  }
  Receive();
}

void TcpConnection::SetAddress() {
  address_ = socket_.remote_endpoint().address().to_string() + ':' + std::to_string(socket_.remote_endpoint().port());
}

}  // namespace interaction
