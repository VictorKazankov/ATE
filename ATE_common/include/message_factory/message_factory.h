#ifndef ATE_COMMON_MESSAGE_FACTORY_H_
#define ATE_COMMON_MESSAGE_FACTORY_H_

#include <string>

#include <jsoncpp/json/writer.h>

namespace vhat_common {
namespace jmsg {

class MessageFactory {
 public:
  // messages client -> server
  static std::string CreateAttachToApplicationRequest(uint16_t timeout, int id);
  static std::string CreateWaitForObjectRequest(const std::string& icon_name, uint32_t timeout, int id);
  static std::string CreateTapObjectRequest(uint16_t x, uint16_t y, int id);
  static std::string CreateTapObjectRequest(uint16_t x, uint16_t y, uint16_t width, uint16_t hight, int id);

  // messages server -> client
  static std::string CreateAttachToApplicationResponse(const std::string& host, uint16_t port, bool is_running, int id);
  static std::string CreateWaitForObjectResponse(uint16_t x, uint16_t y,
                                                 uint16_t width, uint16_t hight, int id);
};

}  // namespace jmsg
}  // namespace vhat_common

#endif  // ATE_COMMON_MESSAGE_FACTORY_H_
