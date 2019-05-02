#include "server/tcp_connection.h"

#include <iostream>

namespace interaction {

TcpConnection::TcpConnection(boost::asio::io_context& service) : socket_(service), read_stream_(&read_buffer_) {}

std::string TcpConnection::Address() const { return address_; }

void TcpConnection::Start() {
  if (!running_) {
    SetAddress();
    running_ = true;
    std::clog << "[INFO] Listening to messages from - " << Address() << std::endl;
    Receive();
  }
}

void TcpConnection::Stop() {
  if (running_) {
    running_ = false;
    std::clog << "[INFO] Stopping client - " << Address() << std::endl;
    socket_.cancel();
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
  std::clog << "[DEBUG] Sending message - " << message_queue_.front() << " - to client - " << Address() << std::endl;
  boost::asio::async_write(socket_, boost::asio::buffer(message_queue_.front()), std::move(handler));
}

void TcpConnection::OnSend(const boost::system::error_code& error, std::size_t bytes_transferred) {
  message_queue_.pop();

  if (error) {
    if (error == boost::asio::error::eof || error == boost::asio::error::connection_reset) {
      std::clog << "[INFO] Client - " << Address() << " - disconnected" << std::endl;
    } else {
      if (error != boost::system::errc::operation_canceled) {
        std::clog << "[ERROR] Error on receiving message from - " << Address() << " - " << error.message() << std::endl;
      }
    }
    Stop();
    return;
  }

  std::clog << "[DEBUG] Sent " << bytes_transferred << " bytes" << std::endl;
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
      std::clog << "[INFO] Client - " << Address() << " - disconnected" << std::endl;
    } else {
      if (error != boost::system::errc::operation_canceled) {
        std::cerr << "[EROR] Error on receiving message from - " << Address() << " - " << error.message() << std::endl;
      }
    }
    
    Stop();
    return;
  }
  std::string message;
  std::getline(read_stream_, message);

  std::clog << "[DEBUG] Received message : \'" << message << "\' : " << bytes_transferred << " bytes from - "
            << Address() << std::endl;

  // TODO (MShvaiko@luxoft.com) : message parsing
  // TODO : remove 'Send("Hello from server!")'
  // Test sending of messages to client
  Send("Hello from server!");
  Receive();
}

void TcpConnection::SetAddress() {
  address_ = socket_.remote_endpoint().address().to_string() + ':' + std::to_string(socket_.remote_endpoint().port());
}

}  // namespace interaction