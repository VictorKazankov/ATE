#include "server/tcp_server.h"

#include <iostream>

namespace interaction {

TcpServer::TcpServer(boost::asio::io_context& context, uint16_t port)
    : context_(context), acceptor_(context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
    handler_(std::make_shared<TcpSessionHandler>()) {
  boost::system::error_code error;
  acceptor_.set_option(boost::asio::socket_base::reuse_address(true), error);
  if (error) {
    std::clog << "[ERROR] " << error.message() << std::endl;
  }
}

void TcpServer::Start() {
  if (!running_) {
    running_ = true;
    std::clog << "[INFO] Starting tcp server on port - " << acceptor_.local_endpoint().port() << std::endl;
    Accept();
  }
}

void TcpServer::Stop() {
  if (running_) {
    running_ = false;
    std::clog << "[INFO] Stopping tcp server" << std::endl;
    boost::system::error_code error;
    acceptor_.close(error);
    if (error) {
      std::clog << "[ERROR] " << error.message() << std::endl;
    }
  }
}

void TcpServer::Accept() {
  if (!running_) return;
  std::clog << "[INFO] Accepting connections" << std::endl;

  auto connection = TcpConnection::Create(context_);
  auto handler = [self = shared_from_this(), connection](auto&&... params) {
    self->OnAccept(connection, std::forward<decltype(params)>(params)...);
  };
  acceptor_.async_accept(connection->Socket(), std::move(handler));
}


void TcpServer::OnAccept(const TcpConnection::TcpConnectionPtr& connection,
                                 const boost::system::error_code& error) {
  if (error) {
    if (error != boost::system::errc::operation_canceled) {
      std::clog << "[ERROR] Error on accepting connection: " << error.message() << std::endl;
    }
  }
  connection->SetHandler(handler_);
  handler_->OnOpen(connection);
  Accept();
}

}  // namespace interaction