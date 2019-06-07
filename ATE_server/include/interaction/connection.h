#ifndef ATE_SERVER_INTERACTION_ATE_CONNECTION_H_
#define ATE_SERVER_INTERACTION_ATE_CONNECTION_H_

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
  /**
   * @brief Send command or messages
  */
  virtual void Send(const std::string& command) = 0;

};

}  // namespace interaction

#endif  // ATE_SERVER_INTERACTION_ATE_TCP_CONNECTION_H_
