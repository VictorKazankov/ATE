#ifndef ATE_SERVER_INTERACTION_ATE_TCP_CONNECTION_MANAGER_H_
#define ATE_SERVER_INTERACTION_ATE_TCP_CONNECTION_MANAGER_H_

#include <functional>
#include <memory>

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>

#include "ate_message_adapter.h"
#include "interaction/connection_manager.h"
#include "tcp_connection.h"
#include "transport_adapter.h"

namespace interaction {

/**
 * @class TcpConnectionManager
 * @brief Asynchronous TCP server
 * This class is accepting clients and creates tcp sessions.
 */
class TcpConnectionManager : public std::enable_shared_from_this<TcpConnectionManager>, public ConnectionManager {
 public:
  /**
   * @brief Creates tcp connection manager (server) instance.
   * @param service - an instannce of io_context provides the core I/O functionality for users of the asynchronous I/O
   * objects.
   * @param port - value of tcp accepting port
   * @param ate_message_adapter - reference to AteMessageAdapter
   * @return pointer to created connection manager instance
   */
  static std::shared_ptr<TcpConnectionManager> Create(boost::asio::io_context& service, uint16_t port,
                                                      IMessageAdapter& ate_message_adapter);

  /**
   * @brief Start running of the TCP connection manager
   */
  void Start() override;

  /**
   * @brief Stop running of the TCP connection manager
   */
  void Stop() override;

 private:
  /**
   * @brief constructor of TcpConnectionManager
   * @param service - an instance of io_context provides the core I/O functionality for users of the asynchronous I/O
   * objects.
   * @param port - value of tcp accepting port
   * @param ate_message_adapter - reference to AteMessageAdapter
   */
  TcpConnectionManager(boost::asio::io_context& service, uint16_t port, IMessageAdapter& ate_message_adapter);

  /**
   * @brief Accept new client connections
   */
  void Accept();
  /**
   * @brief Handler function for async accept
   */
  void OnAccept(const TcpConnection::TcpConnectionPtr& connection, const boost::system::error_code& error);

  boost::asio::io_context& context_;
  boost::asio::ip::tcp::acceptor acceptor_;
  std::shared_ptr<TransportAdapter> transport_adapter_;
  volatile bool running_{false};
};  // namespace interaction

}  // namespace interaction

#endif  // ATE_SERVER_INTERACTION_ATE_TCP_CONNECTION_MANAGER_H_
