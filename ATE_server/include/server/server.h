#ifndef ATE_SERVER_SERVER_H_
#define ATE_SERVER_SERVER_H_

namespace interaction {

/**
 * @class Server
 *
 * @brief Interface class for Servers
 */
class Server {
 public:
  virtual ~Server() = default;
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

#endif  // ATE_SERVER_SERVER_H_
