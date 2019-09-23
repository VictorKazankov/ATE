#ifndef ATE_SERVER_TRANSPORT_ADAPTERS_COLLECTION_H_
#define ATE_SERVER_TRANSPORT_ADAPTERS_COLLECTION_H_

#include <memory>

#include <boost/asio/io_context.hpp>

#include "interaction/connection_manager.h"

/**
 * @class TransportAdaptersCollection
 * @brief Contains ConnectionManagers for communication with external components and pass messages into
 * AteMessageAdapter
 **/
class TransportAdaptersCollection {
 public:
  TransportAdaptersCollection() = default;
  ~TransportAdaptersCollection();

  TransportAdaptersCollection(const TransportAdaptersCollection&) = delete;
  TransportAdaptersCollection& operator=(const TransportAdaptersCollection&) = delete;

  /**
   * @brief Runs client managers for communication with external components
   */
  void Run();

  /**
   * @brief Init tcp connection manager for communication via TCP protocol
   * @param io_context - an instannce of io_context provides the core I/O functionality for users of the asynchronous
   * I/O objects.
   * @param port - value of tcp accepting port
   */
  void InitTcpConnectionManager(boost::asio::io_context& io_context, uint16_t port);

 private:
  std::shared_ptr<interaction::ConnectionManager> tcp_server_;
};

#endif  // ATE_SERVER_TRANSPORT_ADAPTERS_COLLECTION_H_
