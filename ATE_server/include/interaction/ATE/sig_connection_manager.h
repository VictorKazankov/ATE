#ifndef ATE_SERVER_INTERACTION_ATE_SIG_CONNECTION_MANAGER_H_
#define ATE_SERVER_INTERACTION_ATE_SIG_CONNECTION_MANAGER_H_

#include <atomic>
#include <thread>

#include <boost/asio/io_context.hpp>

#include "interaction/connection_manager.h"

class IMessageAdapter;

namespace interaction {

class SigConnectionManager : public ConnectionManager {
 public:
  explicit SigConnectionManager(boost::asio::io_context& io_context, IMessageAdapter& ate_message_adapter);

  SigConnectionManager(const SigConnectionManager&) = delete;
  SigConnectionManager& operator=(const SigConnectionManager&) = delete;

  ~SigConnectionManager() override;

  /**
   * @brief Setup handlers for signals
   */
  void Start() override;

  /**
   * @brief Restore default handlers for signals
   */
  void Stop() override;

 private:
  /**
   * @brief The function reloads icon storage
   */
  void ReloadStorage();

  /**
   * @brief Provide io_context service stop
   */
  void Terminate();

  boost::asio::io_context& io_context_;
  IMessageAdapter& ate_message_adapter_;
  volatile std::atomic_bool running_;
  std::thread loop_;
};

}  // namespace interaction

#endif  // ATE_SERVER_INTERACTION_ATE_SIG_CONNECTION_MANAGER_H_
