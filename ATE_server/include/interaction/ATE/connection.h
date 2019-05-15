#ifndef ATE_SERVER_INTERACTION_ATE_CONNECTION_H_
#define ATE_SERVER_INTERACTION_ATE_CONNECTION_H_

#include <string>

namespace interaction {

/**
 * @class Connection
 *
 * @brief Interface class for client/server Connections
 */
class Connection {
 public:
  virtual ~Connection() = default;
  /**
   * @brief Start session
   */
  virtual void Start() = 0;
  /**
   * @brief Stop session
   */
  virtual void Stop() = 0;
  // TODO : Create virtual function for sending messages
};

}  // namespace interaction

#endif  // ATE_SERVER_INTERACTION_ATE_TCP_CONNECTION_H_
