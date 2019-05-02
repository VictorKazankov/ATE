#ifndef ATE_SERVER_TCP_CONNECTION_H_
#define ATE_SERVER_TCP_CONNECTION_H_

#include <atomic>
#include <istream>
#include <memory>
#include <queue>
#include <string>

#include <boost/asio.hpp>

namespace interaction {

/**
 * \class TcpConnection
 *
 * \brief session class provide interaction between client and server (send/recv) messages
 *
 */
class TcpConnection : public std::enable_shared_from_this<TcpConnection> {
 public:
  using TcpConnectionPtr = std::shared_ptr<TcpConnection>;

  /**
   * \brief Creates instance of session.
   *
   * \param service - an instannce of io_context provides the core I/O functionality for users of the asynchronous I/O
   * objects.
   * \return pointer to created session
   */
  static TcpConnectionPtr Create(boost::asio::io_context& service) { return TcpConnectionPtr(new TcpConnection(service)); }

  /**
   * \brief Get reference on socket
   * \return socket ref for setting up of session from tcp server
   */
  auto& Socket() { return socket_; }

  /**
   * \brief Get uri of client
   * \return string representation uri of client
   */
  std::string Address() const;

  /**
   * \brief Start session
   */
  void Start();

  /**
   * \brief Stop session
   */
  void Stop();

  // TODO (MShvaiko@luxoft.com) : need to use inside Handling class
  /**
   * \brief Send message to client (push messages into message queue)
   * \param message - string representation of message
   */
  void Send(const std::string& message);

 private:
  /**
   * \brief Creates instance of session.
   *
   * \param service - an instannce of io_context provides the core I/O functionality for users of the asynchronous I/O
   * objects.
   * \return pointer to created session
   */
  TcpConnection(boost::asio::io_context& service);

  /**
   * \brief Set address of connection
   */
  void SetAddress();

  /**
   * \brief Receive message from client
   */
  void Receive();

  /**
   * \brief handler of async recv
   * \param error - using inside OnReceive for handling error cases
   * \param bytes_transferred - bytes received from client
   */
  void OnReceive(const boost::system::error_code& error, std::size_t bytes_transferred);

  /**
   * \brief Extract message from message queue and send it to client
   */
  void SendImpl();

  /**
   * \brief Extract messages from message queue and send it to client
   * \param error - using inside OnReceive for handling error cases
   * \param bytes_transferred - bytes received from client
   */
  void OnSend(const boost::system::error_code& error, std::size_t bytes_transferred);

  boost::asio::ip::tcp::socket socket_;
  boost::asio::streambuf read_buffer_;
  std::istream read_stream_;
  std::queue<std::string> message_queue_;
  std::string address_;
  bool running_{false};
};

}  // namespace server

#endif  // ATE_SERVER_TCP_CONNECTION_H_
