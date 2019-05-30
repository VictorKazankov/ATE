#include "interaction/ATE/tcp_server.h"

#include "logger/logger.h"

namespace interaction {

TcpServer::TcpServer(boost::asio::io_context& context, uint16_t port)
    : context_(context),
      acceptor_(context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
      handler_(std::make_shared<TcpSessionHandler>(context)) {
  boost::system::error_code error;
  acceptor_.set_option(boost::asio::socket_base::reuse_address(true), error);
  if (error) {
    logger::error("[tcpserver] Set option failure: {}", error.message());
  }
}

void TcpServer::Start() {
  if (!running_) {
    running_ = true;
    logger::info("[tcpserver] Starting tcp server on port - {}", acceptor_.local_endpoint().port());
    Accept();
  }
}

void TcpServer::Stop() {
  if (running_) {
    running_ = false;
    logger::info("[tcpserver] Stopping tcp server");
    boost::system::error_code error;
    acceptor_.close(error);
    if (error) {
      logger::error("[tcpserver] Stopping acceptor is failure: ", error.message());
    }
  }
}

void TcpServer::Accept() {
  if (!running_) return;
  logger::info("[tcpserver] Accepting connections");

  auto connection = TcpConnection::Create(context_);
  auto handler = [self = shared_from_this(), connection](auto&&... params) {
    self->OnAccept(connection, std::forward<decltype(params)>(params)...);
  };
  acceptor_.async_accept(connection->Socket(), std::move(handler));
}

void TcpServer::OnAccept(const TcpConnection::TcpConnectionPtr& connection, const boost::system::error_code& error) {
  if (error) {
    if (error != boost::system::errc::operation_canceled) {
      logger::error("[tcpserver] Error on accepting connection: {}", error.message());
    }
  }
  connection->SetHandler(handler_);
  handler_->OnOpen(*connection);
  Accept();
}

}  // namespace interaction
