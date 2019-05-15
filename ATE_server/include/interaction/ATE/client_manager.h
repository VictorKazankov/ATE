#ifndef ATE_SERVER_INTERACTION_ATE_CLIENT_MANAGER_H_
#define ATE_SERVER_INTERACTION_ATE_CLIENT_MANAGER_H_

namespace interaction {

/**
 * @class ClientManager
 *
 * @brief Interface class for ClientManagers (Servers)
 */
class ClientManager {
 public:
  virtual ~ClientManager() = default;
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

#endif  // ATE_SERVER_INTERACTION_ATE_CLIENT_MANAGER_H_
