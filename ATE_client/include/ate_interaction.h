#ifndef ATE_INTERACTION_H_
#define ATE_INTERACTION_H_

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>

#include "squish.h"

namespace interaction {

class ATEInteraction {
 private:
  const std::string host_;
  const std::string port_;

  boost::asio::ip::tcp::socket socket_;
  boost::asio::ip::tcp::resolver resolver_;

  size_t reconnect_attempts{3};

  static const size_t kReceiverBuffer{1024};
  char read_buffer_[kReceiverBuffer];

 public:
  ATEInteraction(boost::asio::io_context& io_context, const std::string& host, const std::string& port);
  ~ATEInteraction() = default;

  void Connect();

  squish::Object SendCommand(const std::string& command);

 private:
  void Reconnect();
};
}  // namespace interaction
#endif  // ATE_INTERACTION_H_
