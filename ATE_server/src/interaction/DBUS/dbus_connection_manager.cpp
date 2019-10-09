#include "interaction/DBUS/dbus_connection_manager.h"

#include <functional>

#include <dbus-1.0/dbus/dbus-glib-lowlevel.h>
#include <dbus-1.0/dbus/dbus-glib.h>

#include "logger/logger.h"
#include "message_factory/json_defines.h"
#include "message_factory/json_messages.h"
#include "message_factory/message_factory.h"

namespace interaction {

/*---------------DBus callback function---------------*/
template <typename T>
struct Callback;

template <typename Ret, typename... Params>
struct Callback<Ret(Params...)> {
  template <typename... Args>
  static Ret callback(Args... args) {
    return callback_method(args...);
  }
  static std::function<Ret(Params...)> callback_method;
};

template <typename Ret, typename... Params>
std::function<Ret(Params...)> Callback<Ret(Params...)>::callback_method;

/*---------------DBus callback function---------------*/

DBusConnectionManager::DBusConnectionManager(const std::string& interface, AteMessageAdapter& ate_message_adapter)
    : interface_(interface),
      ate_message_adapter_(ate_message_adapter),
      loop_(g_main_loop_new(nullptr, FALSE), g_main_loop_quit) {
  DBusError error;
  dbus_error_init(&error);

  DBusConnection* connection_ = dbus_bus_get(DBUS_BUS_SYSTEM, &error);

  if (dbus_error_is_set(&error)) {
    logger::critical("[dbus connection manager] Cannot get System BUS connection: {}", error.message);
    dbus_error_free(&error);
    return;
  }

  // Bind g_main_loop and DBus connection
  dbus_connection_setup_with_g_main(connection_, nullptr);
  std::string rule = "interface='" + interface + "'";
  logger::info("[dbus connection manager] Signal match rule: {}", rule);
  dbus_bus_add_match(connection_, rule.c_str(), &error);

  if (dbus_error_is_set(&error)) {
    logger::critical("[dbus connection manager] Cannot add D-BUS match rule, cause: {}", error.message);
    dbus_error_free(&error);
    return;
  }

  // Register DBus callback
  Callback<DBusHandlerResult(DBusConnection*, DBusMessage*, void*)>::callback_method =
      [this](DBusConnection* connection, DBusMessage* message, void* user_data) {
        return DisplayTypeChangedFilter(connection, message, user_data);
      };
  auto callback_method = static_cast<DBusHandleMessageFunction>(
      Callback<DBusHandlerResult(DBusConnection*, DBusMessage*, void*)>::callback);

  dbus_connection_add_filter(connection_, callback_method, nullptr, nullptr);
}

void DBusConnectionManager::Start() { dbus_thread_ = std::thread(g_main_loop_run, loop_.get()); }

void DBusConnectionManager::Stop() {
  loop_.reset();

  if (dbus_thread_.joinable()) {
    dbus_thread_.join();
  }
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

    request_ = common::jmsg::MessageFactory::DBusConnection::CreateDisplayTypeChangedRequest(x, y, GenCorrelationId());

    // Send message
    if (!request_.empty()) {
      ate_message_adapter_.OnMessage(request_);
      request_.clear();
      return DBUS_HANDLER_RESULT_HANDLED;
    }
  }

  return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
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

uint64_t DBusConnectionManager::GenCorrelationId() {
  static uint64_t kCorrelationId = 1;
  return kCorrelationId++;
}

}  // namespace interaction
