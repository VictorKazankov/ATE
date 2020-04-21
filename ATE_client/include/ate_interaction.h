#ifndef ATE_INTERACTION_H_
#define ATE_INTERACTION_H_

#include <memory>

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/streambuf.hpp>

#include "interaction.h"

namespace interaction {

class ATEInteraction : public Interaction {
 public:
  ATEInteraction(std::shared_ptr<boost::asio::io_context> io_context, const std::string& host, const std::string& port);
  ~ATEInteraction() = default;

  /**
   * @brief Function provides host
   * Named according to squish API
   * @return host
   **/
  std::string host() const override { return host_; }

  /**
   * @brief Function provides port
   * Named according to squish API
   * @return port
   **/
  std::string port() const override { return port_; }

  /**
   * @brief Function determine whether the socket is open.
   * @return True if socket is open otherwise false.
   **/
  bool IsConnectionOpened() const override;

  /**
   * @brief Send command to server
   * @param command Specifies string command
   * @return Response string
   */
  std::string SendCommand(const std::string& command) override;

 private:
  /**
   * @brief Function establishes the connection
   */
  void Connect();

  void Reconnect();

  const std::string host_;
  const std::string port_;

  std::shared_ptr<boost::asio::io_context> io_context_;
  mutable boost::asio::ip::tcp::socket socket_;
  boost::asio::ip::tcp::resolver resolver_;

  static const size_t kAttempts{3};
  size_t reconnect_attempts{kAttempts};

  boost::asio::streambuf read_buffer_;
  std::istream read_stream_;
};
}  // namespace interaction

#endif  // ATE_INTERACTION_H_
