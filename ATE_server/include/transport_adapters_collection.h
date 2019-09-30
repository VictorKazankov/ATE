#ifndef ATE_SERVER_TRANSPORT_ADAPTERS_COLLECTION_H_
#define ATE_SERVER_TRANSPORT_ADAPTERS_COLLECTION_H_

#include <memory>
#include <vector>

#include <boost/asio/io_context.hpp>

#include "ate_message_adapter.h"
#include "interaction/connection_manager.h"

/**
 * @class TransportAdaptersCollection
 * @brief Contains ConnectionManagers for communication with external components and pass messages into
 * AteMessageAdapter
 **/
class TransportAdaptersCollection {
 public:
  TransportAdaptersCollection(AteMessageAdapter& ate_message_adapter);
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
  void InitDbusConnectionManager(const std::string& interface);

 private:
  AteMessageAdapter& ate_message_adapter_;
  
  std::vector<std::shared_ptr<interaction::ConnectionManager>> connection_managers_;
};

#endif  // ATE_SERVER_TRANSPORT_ADAPTERS_COLLECTION_H_
