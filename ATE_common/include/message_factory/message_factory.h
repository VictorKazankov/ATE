#ifndef ATE_COMMON_MESSAGE_FACTORY_MESSAGE_FACTORY_H_
#define ATE_COMMON_MESSAGE_FACTORY_MESSAGE_FACTORY_H_

#include <string>

#include "json/value.h"

#include "utils/geometry_types.h"
#include "utils/object_data_type.h"
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
    static std::string CreateAttachToApplicationRequest(int timeout_msec, uint64_t id);
    static std::string CreateWaitForObjectRequest(const std::string& icon_name, int timeout_msec, uint64_t id);
    static std::string CreateWaitForObjectRequest(const common::ObjectDataIdentity& object_data_identity,
                                                  int timeout_msec, uint64_t id);
    static std::string CreateTapObjectRequest(int x, int y, squish::ModifierState modifier_state,
                                              squish::MouseButton mouse_button, uint64_t id);
    static std::string CreateTouchAndDragRequest(const std::string& object_or_name, int x, int y, int dx, int dy,
                                                 squish::ModifierState modifier_state, uint64_t id);
    static std::string CreatePressAndHoldRequest(int x, int y, uint64_t id);
    static std::string CreatePressReleaseRequest(int x, int y, uint64_t id);
    static std::string CreateChangeSyncIconDBRequest(const std::string& sync_version,
                                                     const std::string& sync_build_version, uint64_t id);
    static std::string CreateChangeSyncModeRequest(const std::string& collection_mode, uint64_t id);
    static std::string CreateLongPressRequest(int x, int y, int timeout_msec, uint64_t id);
    static std::string CreateGetScreenshotRequest(const std::string& filename, const std::string& location,
                                                  uint64_t id);
    static std::string CreateGetTextRequest(const common::Point& top_left, const common::Point& bottom_right,
                                            uint64_t id);
    static std::string CreateGetImagesDiscrepancyRequest(const std::string& icon_path_second,
                                                         const std::string& icon_path_first,
                                                         const common::Point& top_left_coordinate,
                                                         const common::Point& bottom_right_coordinate, uint64_t id);
    static std::string CreateGetObjectsDataByPatternRequest(const std::string& object_name, uint64_t id);
    static std::string CreateGetObjectsDataByPatternRequest(const ObjectDataIdentity& object_data_identity,
                                                            uint64_t id);
    static std::string CreateCaptureFramesRequest(int interval, int duration, const common::Point& left_top,
                                                  const common::Point& bottom_right, const std::string& path,
                                                  uint64_t id);
    static std::string CreateFindAllImagesRequest(const std::string& object_name, const common::Point& left_top,
                                                  const common::Point& bottom_right, uint64_t id);
  };

  /**
   * @struct DBusConnection
   * @brief Struct 'DBusConnection' stores factory methods which uses for creating json messages request
   * (dbus adapter -> ATE)
   */
  struct DBusConnection {
    static std::string CreateDisplayTypeChangedRequest(int x, int y, uint64_t id);
    static Json::Value CreateDisplayTypeChangedResponse();
  };

  /**
   * @struct SignalConnection
   * @brief Struct SignalConnection stores factory methods which use for creating json messages request
   */
  struct SignalConnection {
    static std::string CreateIconReloadRequest(uint64_t id);
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
    static Json::Value CreateGetTextResultObject(const std::string& text);
    static Json::Value CreateGetObjectsDataByPatternResponse(const std::vector<ObjectData>& objects_data);
    static Json::Value CreateGetImagesDiscrepancyResponse(int percent_discrepancy);
    static Json::Value CreateCaptureFramesResponse(const std::vector<std::string>& frame_list);
  };
};

}  // namespace jmsg
}  // namespace common

#endif  // ATE_COMMON_MESSAGE_FACTORY_MESSAGE_FACTORY_H_
