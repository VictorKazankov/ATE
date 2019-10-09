#ifndef ATE_SERVER_INTERACTION_DBUS_DBUS_MANAGER_H_
#define ATE_SERVER_INTERACTION_DBUS_DBUS_MANAGER_H_

#include <dbus-1.0/dbus/dbus.h>
#include <glib-2.0/glib.h>

#include <atomic>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <string>
#include <thread>

#include "ate_message_adapter.h"
#include "interaction/connection_manager.h"

namespace interaction {

/**
 * @class DBusConnectionManager
 * @brief Connect to DBus daemon and receive signals
 */
class DBusConnectionManager : public ConnectionManager {
 public:
  DBusConnectionManager(const std::string& interface, AteMessageAdapter& ate_message_adapter);

  /**
   * @brief Start running of the DBus connection manager
   */
  void Start() override;

  /**
   * @brief Stop running of the DBus connection manager
   */
  void Stop() override;

 private:
  /**
   * @brief Generate correlation id for json rpc
   * @return Correlation id
   */
  uint64_t GenCorrelationId();

  /**
   * @brief Filter for signal type 'displayTupeChanged' extract payload, create request and notify 'ChangeResolution'
   * @params DBusConnection for DBus connection (not use)
   * @params message pointer to DBusMessage structure
   * @params void* user data (not use)
   * @return processing message result code
   */
  DBusHandlerResult DisplayTypeChangedFilter(DBusConnection*, DBusMessage* message, void*);

  /**
   * @brief Extract payload from message
   * @params message pointer to DBusMessage structure with payload of signal
   * @params x reference to int (width of the frame)
   * @params y reference to int (height of the frame)
   * @return true in case of success, otherwise - false
   */
  bool ExtractDisplayTypeChangePayload(DBusMessage* message, int& x, int& y);

 private:
  std::string interface_;
  std::string request_;

  AteMessageAdapter& ate_message_adapter_;
  std::thread dbus_thread_;
  std::unique_ptr<GMainLoop, void (*)(GMainLoop*)> loop_;
};

}  // namespace interaction

#endif  // ATE_SERVER_INTERACTION_DBUS_DBUS_MANAGER_H_
