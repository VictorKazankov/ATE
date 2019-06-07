#ifndef ATE_SERVER_INTERACTION_VDP_HTTPS_CLIENT_H_
#define ATE_SERVER_INTERACTION_VDP_HTTPS_CLIENT_H_

#include <string>

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/context.hpp>
#include <boost/asio/ssl/stream.hpp>

#include "interaction/connection.h"

namespace interaction {

class HttpsClient : public Connection {
 private:
  boost::asio::ssl::context ssl_context_{boost::asio::ssl::context::sslv23};
  boost::asio::ssl::stream<boost::asio::ip::tcp::socket> socket_;
  boost::asio::ip::tcp::resolver resolver_;
  boost::system::error_code error_;

  const std::string host_;
  const std::string port_;

 public:
  HttpsClient(boost::asio::io_context& io_context, const std::string& host, const std::string& port);
  ~HttpsClient() override = default;

  /**
   * @brief The function establishes https connection with host
   **/
  void Start() override;

  /**
   * @brief The function closes https connection with host
   **/
  void Stop() override;

  /**
   * @brief Function sends request to the host
   * @param command request to send
   **/
  void Send(const std::string& command) override;

 private:
  std::string PreparePackage(const std::string& command) const;
};
}  // namespace interaction

#endif  // ATE_SERVER_INTERACTION_VDP_HTTPS_CLIENT_H_
