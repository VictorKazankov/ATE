#include "iostream"
#include "squish.h"

namespace squish {
namespace API {
ApplicationContext WaitForApplicationLaunch(std::chrono::seconds /*timeout_secs*/) {
  std::clog << "waitForApplicationLaunch()\n";
  return ApplicationContext();  // TODO implement
}

Object WaitForObject(const std::string& /*object_or_name*/) {
  std::clog << "WaitForObject()\n";
  return Object();  // TODO implement
}

Object WaitForObject(const std::string& /*object_or_name*/, std::chrono::milliseconds /*timeout_msec*/) {
  std::clog << "WaitForObject()\n";
  return Object();  // TODO implement
}

void TapObject(const ScreenPoint& /*screen_point*/) {
  std::clog << "TapObject()\n";
  // TODO implement
}

void TapObject(const ScreenRectangle& /*screen_rectangle*/) {
  std::clog << "TapObject()\n";
  // TODO implement
}

}  // namespace API
}  // namespace squish

int main() {
  std::cout << "ATE server main" << std::endl;

  return EXIT_SUCCESS;
}
