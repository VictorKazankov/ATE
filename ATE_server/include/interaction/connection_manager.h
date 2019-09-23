#ifndef ATE_SERVER_INTERACTION_ATE_CONNECTION_MANAGER_H_
#define ATE_SERVER_INTERACTION_ATE_CONNECTION_MANAGER_H_

namespace interaction {

/**
 * @class ConnectionManager
 * @brief Interface class for ConnectionManager
 */
class ConnectionManager {
 public:
  virtual ~ConnectionManager() = default;
  /**
   * @brief Start running of the server
   */
  virtual void Start() = 0;

  /**
   * @brief Stop running of the server
   */
  virtual void Stop() = 0;
};
}  // namespace interaction

#endif  // ATE_SERVER_INTERACTION_ATE_CONNECTION_MANAGER_H_
