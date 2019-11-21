#ifndef ATE_INTERACTION_H_
#define ATE_INTERACTION_H_

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/streambuf.hpp>

#include "squish/squish_types.h"

namespace interaction {

enum class Method {
  kAttachToApplication,
  kWaitForObject,
  kTapObject,
  kTouchAndDrag,
  kChangeSyncIconDB,
  kChangeSyncMode,
  kLongPress
};

class ATEInteraction {
 private:
  const std::string host_;
  const std::string port_;

  boost::asio::ip::tcp::socket socket_;
  boost::asio::ip::tcp::resolver resolver_;

  static const size_t kAttempts{3};
  size_t reconnect_attempts{kAttempts};

  boost::asio::streambuf read_buffer_;
  std::istream read_stream_;

 public:
  ATEInteraction(boost::asio::io_context& io_context, const std::string& host, const std::string& port);
  ~ATEInteraction() = default;

  void Connect();

  /**
   * @brief Function determine whether the socket is open.
   * @return return true if socket is open otherwise false.
   **/
  bool IsConnectionOpened() const { return socket_.is_open(); }

  squish::Object SendCommand(Method method, const std::string& command);

 private:
  void Reconnect();
};
}  // namespace interaction
#endif  // ATE_INTERACTION_H_
