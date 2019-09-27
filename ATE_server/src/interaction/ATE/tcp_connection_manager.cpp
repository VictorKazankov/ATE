#include "interaction/ATE/tcp_connection_manager.h"

#include "logger/logger.h"

#include "interaction/ATE/tcp_transport_adapter.h"

namespace interaction {

TcpConnectionManager::TcpConnectionManager(boost::asio::io_context& context, uint16_t port, AteMessageAdapter& ate_message_adapter)
    : context_(context),
      acceptor_(context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
      transport_adapter_(std::make_shared<TcpTransportAdapter>(ate_message_adapter)) {
  boost::system::error_code error;
  acceptor_.set_option(boost::asio::socket_base::reuse_address(true), error);
  if (error) {
    logger::error("[tcpconnectionmanager] Set option failure: {}", error.message());
  }
}

std::shared_ptr<TcpConnectionManager> TcpConnectionManager::Create(boost::asio::io_context& service, uint16_t port,
                                             AteMessageAdapter& ate_message_adapter) {
  return std::shared_ptr<TcpConnectionManager>{new TcpConnectionManager{service, port, ate_message_adapter}};
}

void TcpConnectionManager::Start() {
  if (!running_) {
    running_ = true;
    logger::info("[tcpconnectionmanager] Starting tcp server on port - {}", acceptor_.local_endpoint().port());
    Accept();
  }
}

void TcpConnectionManager::Stop() {
  if (running_) {
    running_ = false;
    logger::info("[tcpconnectionmanager] Stopping tcp server");
    boost::system::error_code error;
    acceptor_.close(error);
    if (error) {
      logger::error("[tcpconnectionmanager] Stopping acceptor is failure: ", error.message());
    }
  }
}

void TcpConnectionManager::Accept() {
  if (!running_) return;
  logger::info("[tcpconnectionmanager] Accepting connections");

  auto connection = TcpConnection::Create(context_);
  auto handler = [self = shared_from_this(), connection](auto&&... params) {
    self->OnAccept(connection, std::forward<decltype(params)>(params)...);
  };
  acceptor_.async_accept(connection->Socket(), std::move(handler));
}

void TcpConnectionManager::OnAccept(const TcpConnection::TcpConnectionPtr& connection, const boost::system::error_code& error) {
  if (error) {
    if (error != boost::system::errc::operation_canceled) {
      logger::error("[tcpconnectionmanager] Error on accepting connection: {}", error.message());
    }
  }
  connection->SetHandler(transport_adapter_);
  transport_adapter_->OnOpen(connection);
  Accept();
}

}  // namespace interaction
