#include "iostream"
#include "memory"

#include <pybind11/pybind11.h>

#include <boost/asio/io_context.hpp>
#include "logger/logger.h"

#include "ate_interaction.h"
#include "squish.h"

std::unique_ptr<interaction::ATEInteraction> ate_interaction;

namespace squish {
namespace API {

ApplicationContext WaitForApplicationLaunch(int /*timeout_secs*/) {
  std::clog << "waitForApplicationLaunch()\n";
  if (ate_interaction) {
    std::clog << "Warning ate_interaction already exist\n";
    return ApplicationContext();  // TODO implement
  }

  boost::asio::io_context io_context;

  try {
    ate_interaction = std::make_unique<interaction::ATEInteraction>(io_context, "127.0.0.1", "5000");
  } catch (const boost::system::system_error& boost_error) {
    logger::critical("boost system error: {} ({})", boost_error.what(), boost_error.code());
    exit(EXIT_FAILURE);
  } catch (const std::exception& exception) {
    logger::critical("fatal error: {}", exception.what());
    exit(EXIT_FAILURE);
  }

  io_context.run();

  return ApplicationContext();  // TODO implement
}

Object WaitForObject(const std::string& /*object_or_name*/) {
  std::clog << "WaitForObject()\n";
  return ate_interaction->SendCommand("dump command");  // TODO implement
}

Object WaitForObject(const std::string& /*object_or_name*/, int /*timeout_msec*/) {
  std::clog << "WaitForObject()\n";
  return ate_interaction->SendCommand("dump command");  // TODO implement
}

void TapObject(const ScreenPoint& /*screen_point*/) {
  std::clog << "TapObject()\n";
  ate_interaction->SendCommand("dump command");
  // TODO implement
}

void TapObject(const ScreenRectangle& /*screen_rectangle*/) {
  std::clog << "TapObject()\n";
  ate_interaction->SendCommand("dump command");
  // TODO implement
}

}  // namespace API
}  // namespace squish

namespace py = pybind11;

PYBIND11_MODULE(vhat_client, m) {
  m.doc() = "VHAT plugin for communication with SYNC";
  m.def("waitForApplicationLaunch", &squish::API::WaitForApplicationLaunch,
        "WaitForApplicationLaunch waits for a new application to start up, which Squish then hooks into",
        py::arg("timeout_secs") = squish::API::kDefaultApplicationLaunchTimeout);

  m.def("waitForObject", py::overload_cast<const std::string&>(&squish::API::WaitForObject),
        "waitForObject waits until the objectOrName object is accessible (i.e., it exists and is visible and enabled). "
        "The function waits for the time defined by the testSettings.waitForObjectTimeout property, that many "
        "milliseconds",
        py::arg("object_or_name"));

  m.def("waitForObject", py::overload_cast<const std::string&, int>(&squish::API::WaitForObject),
        "waitForObject waits until the objectOrName object is accessible (i.e., it exists and is visible and enabled). "
        "The function waits for the time defined by the optional timeoutMSec parameter is used, that many "
        "milliseconds. This function is useful if you want to synchronize your script execution",
        py::arg("object_or_name"), py::arg("timeout_msec"));

  m.def("tapObject", py::overload_cast<const squish::ScreenPoint&>(&squish::API::TapObject),
        "tapObject performs a touch tap at the position specified by screenPoint. Position are in screen global "
        "coordinates",
        py::arg("screen_point"));

  m.def("tapObject", py::overload_cast<const squish::ScreenRectangle&>(&squish::API::TapObject),
        "tapObject performs a touch tap at the center of the rectangle specified by screenRectangle. Position are in "
        "screen global coordinates",
        py::arg("screen_rectangle"));
}
