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

ApplicationContext AttachToApplication(const std::string&) {
  std::clog << "AttachToApplication()\n";
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

Object WaitForObject(const std::string& object_or_name) {
  return WaitForObject(object_or_name, 0);  // TODO implement testSettings.waitForObjectTimeout
}

Object WaitForObject(const Object& object_or_name) {
  return WaitForObject(object_or_name, 0);  // TODO implement testSettings.waitForObjectTimeout
}  // namespace API

Object WaitForObject(const squish::Object& /*object_or_name*/, int /*timeout_msec*/) {
  logger::info("Object WaitForObject()\n");
  return ate_interaction->SendCommand("dump command");  // TODO implement
}

Object WaitForObject(const std::string& /*object_or_name*/, int /*timeout_msec*/) {
  logger::info("Object WaitForObject()\n");
  return ate_interaction->SendCommand("dump command");  // TODO implement
}

void TapObject(const ScreenPoint& /*screen_point*/, ModifierState /*modifier_state*/, MouseButton /*button*/) {
  logger::info("void TapObject()");

  ate_interaction->SendCommand("dump command");  // TODO implement
}

void TapObject(const ScreenRectangle& screen_rectangle, ModifierState modifier_state, MouseButton button) {
  TapObject(screen_rectangle.Center(), modifier_state, button);
}

void TapObject(const Object& screen_rectangle, ModifierState modifier_state, MouseButton button) {
  TapObject(screen_rectangle.Center(), modifier_state, button);
}

}  // namespace API
}  // namespace squish

namespace py = pybind11;

PYBIND11_MODULE(vhat_client, m) {
  m.doc() = "VHAT plugin for communication with SYNC";
  py::class_<squish::Object>(m, "Object")
      .def(py::init())
      .def_readwrite("x", &squish::Object::x)
      .def_readwrite("y", &squish::Object::y)
      .def_readwrite("width", &squish::Object::width)
      .def_readwrite("height", &squish::Object::height);

  py::enum_<squish::ModifierState>(m, "ModifierState")
      .value("NONE", squish::ModifierState::NONE)
      .value("ALT", squish::ModifierState::ALT)
      .value("CONTROL", squish::ModifierState::CONTROL)
      .value("SHIFT", squish::ModifierState::SHIFT);

  py::enum_<squish::MouseButton>(m, "MouseButton")
      .value("NONE", squish::MouseButton::NONE)
      .value("LEFT_BUTTON", squish::MouseButton::LEFT_BUTTON)
      .value("MIDLE_BUTTON", squish::MouseButton::MIDLE_BUTTON)
      .value("RIGHT_BUTTON", squish::MouseButton::RIGHT_BUTTON);

  m.def("attachToApplication", &squish::API::AttachToApplication,
        "This function causes to attach to the application called aut_name and returns a handle to its application "
        "context",
        py::arg("aut_name"));

  m.def("waitForObject", py::overload_cast<const std::string&>(&squish::API::WaitForObject),
        "waitForObject waits until the objectOrName object is accessible (i.e., it exists and is visible and enabled). "
        "The function waits for the time defined by the testSettings.waitForObjectTimeout property, that many "
        "milliseconds",
        py::arg("object_or_name"));

  m.def("waitForObject", py::overload_cast<const squish::Object&>(&squish::API::WaitForObject),
        "waitForObject waits until the objectOrName object is accessible (i.e., it exists and is visible and enabled). "
        "The function waits for the time defined by the testSettings.waitForObjectTimeout property, that many "
        "milliseconds",
        py::arg("object_or_name"));

  m.def("waitForObject", py::overload_cast<const std::string&, int>(&squish::API::WaitForObject),
        "waitForObject waits until the objectOrName object is accessible (i.e., it exists and is visible and enabled). "
        "The function waits for the time defined by the optional timeoutMSec parameter is used, that many "
        "milliseconds. This function is useful if you want to synchronize your script execution",
        py::arg("object_or_name"), py::arg("timeout_msec"));

  m.def("waitForObject", py::overload_cast<const squish::Object&, int>(&squish::API::WaitForObject),
        "waitForObject waits until the objectOrName object is accessible (i.e., it exists and is visible and enabled). "
        "The function waits for the time defined by the optional timeoutMSec parameter is used, that many "
        "milliseconds. This function is useful if you want to synchronize your script execution",
        py::arg("object_or_name"), py::arg("timeout_msec"));

  m.def("tapObject",
        py::overload_cast<const squish::ScreenPoint&, squish::ModifierState, squish::MouseButton>(
            &squish::API::TapObject),
        "tapObject performs a touch tap at the position specified by screenPoint. Position are in screen global "
        "coordinates",
        py::arg("screen_point"), py::arg("modifier_state"), py::arg("button"));

  m.def("tapObject",
        py::overload_cast<const squish::ScreenRectangle&, squish::ModifierState, squish::MouseButton>(
            &squish::API::TapObject),
        "tapObject performs a touch tap at the center of the rectangle specified by screenRectangle. Position are in "
        "screen global coordinates",
        py::arg("screen_rectangle"), py::arg("modifier_state"), py::arg("button"));

  m.def("tapObject",
        py::overload_cast<const squish::Object&, squish::ModifierState, squish::MouseButton>(&squish::API::TapObject),
        "tapObject performs a touch tap at the center of the rectangle specified by screenRectangle. Position are in "
        "screen global coordinates",
        py::arg("screen_rectangle"), py::arg("modifier_state"), py::arg("button"));
}
