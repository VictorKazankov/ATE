#ifndef ATE_SERVER_INTERACTION_ATE_TCP_SERVER_H_
#define ATE_SERVER_INTERACTION_ATE_TCP_SERVER_H_

#include <functional>
#include <memory>

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>

#include "client_manager.h"
#include "tcp_connection.h"
#include "tcp_session_handler.h"

namespace interaction {

/**
 * @class TcpServer
 *
 * @brief Asynchronous TCP server
 *
 * This class is accepting clients and creates tcp sessions.
 *
 */
class TcpServer : public std::enable_shared_from_this<TcpServer>, public ClientManager {
 public:
  /**
   * @brief Creates server instance.
   *
   * @param service - an instannce of io_context provides the core I/O functionality for users of the asynchronous I/O
   * objects.
   * @param port - value of tcp accepting port
   * @return pointer to created server instance
   */
  static std::shared_ptr<TcpServer> Create(boost::asio::io_context& service, uint16_t port);

  /**
   * @brief Start running of the TCP server
   */
  void Start() override;

  /**
   * @brief Stop running of the TCP server
   */
  void Stop() override;

 private:
  /**
   * @brief constructor of TcpServer
   * @param service - an instance of io_context provides the core I/O functionality for users of the asynchronous I/O
   * objects.
   * @param port - value of tcp accepting port
   * @return pointer to created server instance
   */
  TcpServer(boost::asio::io_context& service, uint16_t port);

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
  std::shared_ptr<SessionHandler> handler_;
  volatile bool running_{false};
};  // namespace interaction

}  // namespace interaction

#endif  // ATE_SERVER_INTERACTION_ATE_TCP_SERVER_H_
