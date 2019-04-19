#include "interaction/https_client.h"

#include <functional>
#include <iostream>

namespace interaction {

using boost::asio::ip::tcp;

HttpsClient::HttpsClient(boost::asio::io_context& io_context, boost::asio::ssl::context& ssl_context,
                         boost::asio::ip::basic_resolver_results<tcp> endpoints)
    : socket_(io_context, ssl_context) {
  socket_.set_verify_mode(boost::asio::ssl::verify_none);

  boost::asio::async_connect(socket_.lowest_layer(), endpoints,
                             std::bind(&HttpsClient::HandleConnect, this, std::placeholders::_1));
}

HttpsClient::~HttpsClient() {}

void HttpsClient::HandleConnect(const boost::system::error_code& error) {
  if (!error) {
    socket_.async_handshake(boost::asio::ssl::stream_base::client,
                            std::bind(&HttpsClient::HandleHandshake, this, std::placeholders::_1));
  } else {
    // TODO(KVGrygoriev@luxoft.com) : Change 'clog' to logger system
    std::clog << "Error: Connect failed: " << error.message() << "\n";
  }
}

void HttpsClient::HandleHandshake(const boost::system::error_code& error) {
  if (error) {
    // TODO(KVGrygoriev@luxoft.com) : Change 'clog' to logger system
    std::clog << "Error: Handshake failed: " << error.message() << "\n";
  }
}

void HttpsClient::HandleWrite(const boost::system::error_code& error, size_t bytes_transferred) {
  if (!error) {
    boost::asio::async_read(socket_, boost::asio::buffer(reply_, bytes_transferred),
                            std::bind(&HttpsClient::HandleRead, this, std::placeholders::_1, std::placeholders::_2));
  } else {
    // TODO(KVGrygoriev@luxoft.com) : Change 'clog' to logger system
    std::clog << "Error: Write failed: " << error.message() << "\n";
  }
}

void HttpsClient::HandleRead(const boost::system::error_code& error, size_t bytes_transferred) {
  if (!error) {
    // TODO(KVGrygoriev@luxoft.com) : Change 'clog' to logger system
    std::clog << "Debug: Reply: ";
    std::clog.write(reply_, bytes_transferred);
    std::clog << "\n";
  } else {
    // TODO(KVGrygoriev@luxoft.com) : Change 'clog' to logger system
    std::clog << "Error: Read failed: " << error.message() << "\n";
  }
}

}  // namespace interaction
