#ifndef ATE_SERVER_INTERACTION_ATE_TRANSPORT_ADAPTER_H_
#define ATE_SERVER_INTERACTION_ATE_TRANSPORT_ADAPTER_H_

#include <memory>
#include <string>

#include "interaction/connection.h"

namespace interaction {

/**
 * @class TransportAdapter
 *
 * @brief Interface class adapter between application and external components
 *
 * Class receives request and send responses via method 'OnMessage'
 *
 */
class TransportAdapter {
 public:
  virtual ~TransportAdapter() = default;

  /**
   * @brief Start interaction between client and server calls Connection->Start()
   * @param session - Handle for the accepted connection
   */
  virtual void OnOpen(std::shared_ptr<Connection> session) = 0;

  /**
   * @brief Gets messages from client, prepare and send response
   * @param session - Connection with client
   * @param message - Received message from client connection
   */
  virtual void OnMessage(std::shared_ptr<Connection> session, const std::string& message) = 0;
};

}  // namespace interaction

#endif  // ATE_SERVER_INTERACTION_ATE_TRANSPORT_ADAPTER_H_
