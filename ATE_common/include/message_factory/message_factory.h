#ifndef ATE_COMMON_MESSAGE_FACTORY_MESSAGE_FACTORY_H_
#define ATE_COMMON_MESSAGE_FACTORY_MESSAGE_FACTORY_H_

#include <string>

#include <jsoncpp/json/writer.h>

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
    static std::string CreateTapObjectRequest(uint16_t x, uint16_t y, int id);
    static std::string CreateTapObjectRequest(uint16_t x, uint16_t y, uint16_t width, uint16_t hight, int id);
  };

  /**
   * @struct Server
   * @brief Struct 'Server' stores factory methods which uses for creating json messages response (server -> client)
   */
  struct Server {
    static std::string CreateAttachToApplicationResponse(const std::string& host, uint16_t port, bool is_running,
                                                         int id);
    static std::string CreateWaitForObjectResponse(uint16_t x, uint16_t y, uint16_t width, uint16_t hight, int id);
  };
};

}  // namespace jmsg
}  // namespace common

#endif  // ATE_COMMON_MESSAGE_FACTORY_MESSAGE_FACTORY_H_