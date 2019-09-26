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
  ~DBusConnectionManager() override;

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
   * @brief Filter for signal type 'displayTupeChanged' extract payload, create request and notify 'ChangeResolution'
   * @params DBusConnection is not use
   * @params message pointer to DBusMessage structure
   */
  static DBusHandlerResult DisplayTypeChangedFilter(DBusConnection*, DBusMessage* message, void*);

  /**
   * @brief Extract payload from message
   * @params message pointer to DBusMessage structure with payload of signal
   * @params x reference to int (width of the frame)
   * @params y reference to int (height of the frame)
   */
  static bool ExtractDisplayTypeChangePayload(DBusMessage* message, int& x, int& y);

  /**
   * @brief Wait for notification signal of 'DisplayTypeChangedFilter' and pass request_ to ATE message adapter
   */
  void ChangeResolution();

  static bool notified_;
  static std::atomic_bool running_;
  static std::mutex request_guard_;
  static std::condition_variable condition_;
  static std::string interface_;
  static std::string request_;

  AteMessageAdapter& ate_message_adapter_;
  std::thread dbus_thread_;
  std::thread change_resolution_thread_;
  std::unique_ptr<GMainLoop, void (*)(GMainLoop*)> loop_;
};

}  // namespace interaction

#endif  // ATE_SERVER_INTERACTION_DBUS_DBUS_MANAGER_H_
