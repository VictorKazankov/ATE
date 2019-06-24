#include <pybind11/pybind11.h>
#include <boost/system/system_error.hpp>

#include "error_defines.h"
#include "squish/squish_API.h"

#include "utils/geometry_types.h"
#include "utils/squish_types.h"

namespace py = pybind11;

PYBIND11_MODULE(vhat_client, m) {
  m.doc() = "VHAT plugin for communication with SYNC";

  py::class_<squish::ApplicationContext>(m, "ApplicationContext")
      .def(py::init())
      .def("detach", &squish::ApplicationContext::Detach)
      .def_readonly("host", &squish::ApplicationContext::host)
      .def_readonly("port", &squish::ApplicationContext::port);

  py::class_<common::Point>(m, "ScreenPoint")
      .def(py::init())
      .def(py::init<int&, int&>(), "", py::arg("x"), py::arg("y"))
      .def_readwrite("x", &common::Point::x)
      .def_readwrite("y", &common::Point::y);

  py::class_<common::Rect>(m, "ScreenRectangle")
      .def(py::init())
      .def(py::init<int&, int&, int&, int&>(), "", py::arg("x"), py::arg("y"), py::arg("width"), py::arg("height"))
      .def_readwrite("x", &common::Rect::x)
      .def_readwrite("y", &common::Rect::y)
      .def_readwrite("width", &common::Rect::width)
      .def_readwrite("height", &common::Rect::height);

  py::class_<squish::Object>(m, "Object")
      .def(py::init())
      .def_readwrite("x", &squish::Object::x)
      .def_readwrite("y", &squish::Object::y)
      .def_readwrite("width", &squish::Object::width)
      .def_readwrite("height", &squish::Object::height)
      .def_readwrite("name", &squish::Object::name)
      .def_readwrite("type", &squish::Object::type);

  py::register_exception<squish::LookupError>(m, "LookupError");
  py::register_exception<boost::system::system_error>(m, "boost_system_error");

  py::enum_<common::squish::ModifierState>(m, "ModifierState")
      .value("NONE", common::squish::ModifierState::NONE)
      .value("ALT", common::squish::ModifierState::ALT)
      .value("CONTROL", common::squish::ModifierState::CONTROL)
      .value("SHIFT", common::squish::ModifierState::SHIFT);

  py::enum_<common::squish::MouseButton>(m, "MouseButton")
      .value("NONE", common::squish::MouseButton::NONE)
      .value("LEFT_BUTTON", common::squish::MouseButton::LEFT_BUTTON)
      .value("MIDDLE_BUTTON", common::squish::MouseButton::MIDDLE_BUTTON)
      .value("RIGHT_BUTTON", common::squish::MouseButton::RIGHT_BUTTON);

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
        py::overload_cast<const common::Point&, common::squish::ModifierState, common::squish::MouseButton>(
            &squish::API::TapObject),
        "tapObject performs a touch tap at the position specified by screenPoint. Position are in screen global "
        "coordinates",
        py::arg("screen_point"), py::arg("modifier_state"), py::arg("button"));

  m.def("tapObject",
        py::overload_cast<const common::Rect&, common::squish::ModifierState, common::squish::MouseButton>(
            &squish::API::TapObject),
        "tapObject performs a touch tap at the center of the rectangle specified by screenRectangle. Position are in "
        "screen global coordinates",
        py::arg("screen_rectangle"), py::arg("modifier_state"), py::arg("button"));

  m.def("tapObject",
        py::overload_cast<const squish::Object&, common::squish::ModifierState, common::squish::MouseButton>(
            &squish::API::TapObject),
        "tapObject performs a touch tap at the center of the rectangle specified by screenRectangle. Position are in "
        "screen global coordinates",
        py::arg("screen_rectangle"), py::arg("modifier_state"), py::arg("button"));
}
