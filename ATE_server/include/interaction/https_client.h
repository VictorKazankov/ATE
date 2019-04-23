#ifndef ATE_SERVER_INTERACTION_HTTPS_CLIENT_H_
#define ATE_SERVER_INTERACTION_HTTPS_CLIENT_H_

#include "boost/asio.hpp"
#include "boost/asio/ssl.hpp"

namespace interaction {

namespace {
const size_t kMaxMessageLength = 1024;
}

class HttpsClient {
 private:
  boost::asio::ssl::stream<boost::asio::ip::tcp::socket> socket_;
  char reply_[kMaxMessageLength];

 public:
  HttpsClient(boost::asio::io_context& io_context, boost::asio::ssl::context& ssl_context,
              boost::asio::ip::basic_resolver_results<boost::asio::ip::tcp> endpoints);
  ~HttpsClient();

  void SendCommand(const std::string& command);

 private:
  void HandleConnect(const boost::system::error_code& error);
  void HandleWrite(const boost::system::error_code& error, size_t bytes_transferred);
  void HandleRead(const boost::system::error_code& error, size_t bytes_transferred) const;

  std::string PreparePackage(const std::string& command) const noexcept;
};
}  // namespace interaction

#endif  // ATE_SERVER_INTERACTION_HTTPS_CLIENT_H_