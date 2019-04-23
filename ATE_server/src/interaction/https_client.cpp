#include "interaction/https_client.h"

#include <functional>
#include <iostream>
#include <sstream>

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

std::string HttpsClient::PreparePackage(const std::string& command) const noexcept {
  std::ostringstream request_stream;
  request_stream << "POST /ontouchevent\r\n";
  request_stream << "Host: 172.31.239.130:8444 \r\n";
  request_stream << "Content-Type: application/x-www-form-urlencoded \r\n";
  request_stream << "Content-Length: " << command.length() << "\r\n";
  request_stream << "Accept: text/plain\r\n\r\n";
  request_stream << command;

  return request_stream.str();
}

void HttpsClient::SendCommand(const std::string& command) {
  const std::string request = PreparePackage(command);

  // TODO(KVGrygoriev@luxoft.com) : Change 'clog' to logger system
  std::clog << "Debug: Request to send:\n" << request << "\n";

  boost::asio::async_write(socket_, boost::asio::buffer(request.c_str(), request.length()),
                           std::bind(&HttpsClient::HandleWrite, this, std::placeholders::_1, std::placeholders::_2));
}

void HttpsClient::HandleConnect(const boost::system::error_code& error) {
  if (!error) {
    socket_.async_handshake(boost::asio::ssl::stream_base::client, [this](const boost::system::error_code& error) {
      if (error) {
        // TODO(KVGrygoriev@luxoft.com) : Change 'clog' to logger system
        std::clog << "Error: Handshake failed: " << error.message() << "\n";
      }
    });
  } else {
    // TODO(KVGrygoriev@luxoft.com) : Change 'clog' to logger system
    std::clog << "Error: Connect failed: " << error.message() << "\n";
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

void HttpsClient::HandleRead(const boost::system::error_code& error, size_t bytes_transferred) const {
  if (!error) {
    // TODO(KVGrygoriev@luxoft.com) : Change 'clog' to logger system
    std::clog << "Debug: Reply:\n";
    std::clog.write(reply_, bytes_transferred);
    std::clog << "\n";
  } else {
    // TODO(KVGrygoriev@luxoft.com) : Change 'clog' to logger system
    std::clog << "Error: Read failed: " << error.message() << "\n";
  }
}

}  // namespace interaction
