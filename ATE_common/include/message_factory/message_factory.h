#ifndef ATE_COMMON_MESSAGE_FACTORY_MESSAGE_FACTORY_H_
#define ATE_COMMON_MESSAGE_FACTORY_MESSAGE_FACTORY_H_

#include <string>

#include "json/value.h"

#include "utils/squish_types.h"

namespace common {
namespace jmsg {

/**
 * @class MessageFactory
 * @brief Factory class for creating messages which uses for interaction between client and server
 */
class MessageFactory {
 public:
  /**
   * @struct Client
   * @brief Struct 'Client' stores factory methods which uses for creating json messages requests (client -> server)
   */
  struct Client {
    static std::string CreateAttachToApplicationRequest(uint16_t timeout_msec, int id);
    static std::string CreateWaitForObjectRequest(const std::string& icon_name, uint32_t timeout_msec, int id);
    static std::string CreateTapObjectRequest(uint16_t x, uint16_t y, squish::ModifierState modifier_state,
                                              squish::MouseButton mouse_button, int id);
    static std::string CreateTouchAndDragRequest(const std::string& object_or_name, uint16_t x, uint16_t y, int16_t dx,
                                                 int16_t dy, squish::ModifierState modifier_state, int id);
    static std::string CreatePressAndHoldRequest(uint16_t x, uint16_t y, int id);
    static std::string CreatePressReleaseRequest(uint16_t x, uint16_t y, int id);
    static std::string CreateChangeSyncIconDBRequest(const std::string& sync_version,
                                                      const std::string& sync_build_version, int id);
    static std::string CreateChangeSyncModeRequest(const std::string& collection_mode, int id);
    static std::string CreateLongPressRequest(uint16_t x, uint16_t y, uint32_t timeout_msec, int id);
    static std::string CreateGetScreenshotRequest(const std::string& filename, const std::string& location, int id);
  };

  /**
   * @struct DBusConnection
   * @brief Struct 'DBusConnection' stores factory methods which uses for creating json messages request
   * (dbus adapter -> ATE)
   */
  struct DBusConnection {
    static std::string CreateDisplayTypeChangedRequest(uint16_t x, uint16_t y, int id);
    static Json::Value CreateDisplayTypeChangedResponse();
  };

  /**
   * @struct SignalConnection
   * @brief Struct SignalConnection stores factory methods which use for creating json messages request
   */
  struct SignalConnection {
    static std::string CreateIconReloadRequest(int id);
    static Json::Value CreateIconReloadResponse();
  };

  /**
   * @struct Server
   * @brief Struct 'Server' stores factory methods which uses for creating json messages response (server -> client)
   */
  struct Server {
    static std::string CreateResponse(std::uint64_t id, Json::Value result_or_error, bool is_result);
    static Json::Value CreateTapObjectResultObject();
    static Json::Value CreateTouchAndDragResultObject();
    static Json::Value CreatePressAndHoldResultObject();
    static Json::Value CreatePressReleaseResultObject();
    static Json::Value CreateWaitForObjectResultObject(int x, int y, int width, int height);
    static Json::Value CreateChangeSyncIconDBResultObject();
    static Json::Value CreateChangeSyncModeResultObject();
    static Json::Value CreateLongPressResultObject();
    static Json::Value CreateGetScreenshotObject();
  };
};

}  // namespace jmsg
}  // namespace common

#endif  // ATE_COMMON_MESSAGE_FACTORY_MESSAGE_FACTORY_H_
