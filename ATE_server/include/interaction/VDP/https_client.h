#ifndef ATE_SERVER_INTERACTION_VDP_HTTPS_CLIENT_H_
#define ATE_SERVER_INTERACTION_VDP_HTTPS_CLIENT_H_

#include <memory>
#include <queue>

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/context.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <boost/asio/steady_timer.hpp>

// TODO: Move this file into ATE_ccommon
#include "interaction/ATE/connection.h"

namespace interaction {

class HttpsClient : public std::enable_shared_from_this<HttpsClient>, public Connection {
 public:
  using HttpsClientShared = std::shared_ptr<HttpsClient>;

 private:
  boost::asio::ssl::context ssl_context_{boost::asio::ssl::context::sslv23};
  boost::asio::ssl::stream<boost::asio::ip::tcp::socket> socket_;
  boost::asio::ip::tcp::resolver resolver_;

  boost::asio::steady_timer timer_;

  const std::string host_;
  const std::string port_;

  size_t reconnect_attempts = {3};

  std::queue<std::string> message_queue_;

 public:
  static HttpsClientShared CreateHttpsClient(boost::asio::io_context& io_context, const std::string& host,
                                             const std::string& port) {
    return HttpsClientShared(new HttpsClient(io_context, host, port));
  }
  ~HttpsClient() = default;

  /**
   * @brief The function establishes https connection with host
   **/
  void Start();

  /**
   * @brief The function closes https connection with host
   **/
  void Stop();

  /**
   * @brief Function sends request to the host
   * @param command request to send
   **/
  void SendCommand(const std::string& command);

  /**
   * @brief Function sends request to the host
   * @param first_command request to send
   * @param second_command additional request to send. Uses for avoid network overhead for sending twin request.
   * For example "Tap" consist from "Press" and "Release" requests.
   **/
  void SendCommand(const std::string& first_command, const std::string& second_command);

 private:
  HttpsClient(boost::asio::io_context& io_context, const std::string& host, const std::string& port);

  void Resolve();
  void OnResolve(const boost::system::error_code& error, boost::asio::ip::tcp::resolver::results_type results);

  void OnConnect(const boost::system::error_code& error, boost::asio::ip::tcp::resolver::results_type::iterator);
  void Reconnect();
  void OnReconnect(const boost::system::error_code& error);

  void OnSend(const boost::system::error_code& error, std::size_t bytes_transferred);
  void OnHandshake(const boost::system::error_code& error);

  void SendImpl();

  std::string PreparePackage(const std::string& command) const;
};
}  // namespace interaction

#endif  // ATE_SERVER_INTERACTION_VDP_HTTPS_CLIENT_H_
