#include "interaction/DBUS/dbus_connection_manager.h"

#include <dbus-1.0/dbus/dbus-glib-lowlevel.h>
#include <dbus-1.0/dbus/dbus-glib.h>

#include "logger/logger.h"
#include "message_factory/json_defines.h"
#include "message_factory/json_messages.h"
#include "message_factory/message_factory.h"

namespace {
uint64_t kCorrelationId = 1;
static uint64_t GetCorrelationId() { return kCorrelationId++; }
}  // namespace

namespace interaction {
bool DBusConnectionManager::notified_{};
std::atomic_bool DBusConnectionManager::running_{};
std::mutex DBusConnectionManager::request_guard_;
std::condition_variable DBusConnectionManager::condition_;
std::string DBusConnectionManager::interface_ = "";
std::string DBusConnectionManager::request_ = "";

DBusConnectionManager::DBusConnectionManager(const std::string& interface, AteMessageAdapter& ate_message_adapter)
    : ate_message_adapter_(ate_message_adapter), loop_(g_main_loop_new(NULL, FALSE), g_main_loop_unref) {
  interface_ = interface;
  DBusError error;
  dbus_error_init(&error);

  DBusConnection* connection_ = dbus_bus_get(DBUS_BUS_SYSTEM, &error);

  if (dbus_error_is_set(&error)) {
    logger::critical("[dbus connection manager] Cannot get System BUS connection: {}", error.message);
    dbus_error_free(&error);
    return;
  }

  dbus_connection_setup_with_g_main(connection_, NULL);
  std::string rule = "interface='" + interface + "'";
  logger::info("[dbus connection manager] Signal match rule: {}", rule);
  dbus_bus_add_match(connection_, rule.c_str(), &error);

  if (dbus_error_is_set(&error)) {
    logger::critical("[dbus connection manager] Cannot add D-BUS match rule, cause: {}", error.message);
    dbus_error_free(&error);
    return;
  }

  dbus_connection_add_filter(connection_, &DBusConnectionManager::DisplayTypeChangedFilter, NULL, NULL);
}

DBusConnectionManager::~DBusConnectionManager() { Stop(); }

void DBusConnectionManager::Start() {
  running_ = true;
  dbus_thread_ = std::thread(g_main_loop_run, loop_.get());
  change_resolution_thread_ = std::thread(&DBusConnectionManager::ChangeResolution, this);
}

void DBusConnectionManager::Stop() {
  running_ = false;
  loop_.reset();
  if (change_resolution_thread_.joinable()) {
    change_resolution_thread_.join();
  }
  if (dbus_thread_.joinable()) {
    dbus_thread_.join();
  }
}

void DBusConnectionManager::ChangeResolution() {
  std::unique_lock<std::mutex> lock(request_guard_);

  while (running_) {
    condition_.wait(lock, [&] { return notified_; });
    if (!request_.empty()) {
      ate_message_adapter_.OnMessage(request_);
      notified_ = false;
      request_ = "";
    }
  }
}

bool DBusConnectionManager::ExtractDisplayTypeChangePayload(DBusMessage* message, int& x, int& y) {
  DBusError error;
  DBusMessageIter args;
  dbus_error_init(&error);

  char* key;

  if (!dbus_message_iter_init(message, &args)) {
    // arguments should be present
    logger::error("[dbus connection manager] Message has no argument");
    return false;
  }

  if (DBUS_TYPE_ARRAY == dbus_message_iter_get_arg_type(&args)) {
    DBusMessageIter dict_entry;
    DBusMessageIter dict_key;
    DBusMessageIter dict_value;

    do {
      dbus_message_iter_recurse(&args, &dict_entry);

      do {
        if (DBUS_TYPE_DICT_ENTRY == dbus_message_iter_get_arg_type(&dict_entry)) {
          dbus_message_iter_recurse(&dict_entry, &dict_key);

          if (DBUS_TYPE_STRING == dbus_message_iter_get_arg_type(&dict_key)) {
            dbus_message_iter_get_basic(&dict_key, &key);
            dbus_message_iter_next(&dict_key);

            dbus_message_iter_recurse(&dict_key, &dict_value);

            if (std::string(key) == common::jmsg::kAbscissa &&
                DBUS_TYPE_UINT32 == dbus_message_iter_get_arg_type(&dict_value)) {
              dbus_message_iter_get_basic(&dict_value, &x);
            }

            if (std::string(key) == common::jmsg::kOrdinate &&
                DBUS_TYPE_UINT32 == dbus_message_iter_get_arg_type(&dict_value)) {
              dbus_message_iter_get_basic(&dict_value, &y);
            }
          }
        }

      } while (dbus_message_iter_next(&dict_entry));
    } while (dbus_message_iter_next(&args));
  }

  return true;
}

DBusHandlerResult DBusConnectionManager::DisplayTypeChangedFilter(DBusConnection*, DBusMessage* message, void*) {
  bool result{};
  int x{};
  int y{};

  if (dbus_message_is_signal(message, interface_.c_str(), common::jmsg::kDisplayTypeChanged)) {
    // read the parameters
    result = ExtractDisplayTypeChangePayload(message, x, y);

    if (!result || !x || !y) {
      logger::error("[dbus connection manager] Failed to extract arguments for change resolution");
      return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
    }

    std::unique_lock<std::mutex> lock(request_guard_);
    request_ = common::jmsg::MessageFactory::DBusConnection::CreateDisplayTypeChangedRequest(x, y, GetCorrelationId());
    notified_ = true;
    condition_.notify_all();
    return DBUS_HANDLER_RESULT_HANDLED;
  }

  return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
}

}  // namespace interaction
