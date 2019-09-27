#ifndef ATE_SERVER_INTERACTION_ATE_TCP_SESSION_HANDLER_H_
#define ATE_SERVER_INTERACTION_ATE_TCP_SESSION_HANDLER_H_

#include <string>

#include "ate_message_adapter.h"
#include "interaction/ATE/transport_adapter.h"

namespace interaction {

/**
 * @class TcpTransportAdapter
 *
 * @brief Class receives messages and provide them for AteMessageAdapter
 *
 * Class receives request and send responses via method 'OnMessage' over TCP protocol
 *
 */
class TcpTransportAdapter : public TransportAdapter {
 public:
  TcpTransportAdapter(AteMessageAdapter& ate_adapter);
  ~TcpTransportAdapter() override = default;

  /**
   * @brief Start interaction between client and server calls TcpConnectionPtr->Start()
   * @param session - Handle for the accepted connection
   */
  void OnOpen(std::shared_ptr<Connection> session) override;

  /**
   * @brief Gets requests from client and send response
   * @param session - Connection with client
   * @param message - Received message from client connection
   */
  void OnMessage(std::shared_ptr<Connection> session, const std::string& message) override;

 private:
  AteMessageAdapter& ate_message_adapter_;
};

}  // namespace interaction

#endif  // ATE_SERVER_INTERACTION_ATE_TCP_SESSION_HANDLER_H_
