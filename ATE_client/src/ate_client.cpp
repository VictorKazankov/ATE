#include <pybind11/pybind11.h>
#include <boost/system/system_error.hpp>

#include "error_defines.h"
#include "squish/squish_API.h"

#include "utils/geometry_types.h"
#include "utils/squish_types.h"

namespace py = pybind11;

namespace {
const uint32_t kDefaultLongPressTimeout = 2000;
} // namespace

PYBIND11_MODULE(vhat_client, m) {
  m.doc() = "ATE plugin for communication with SYNC";

  py::class_<squish::ApplicationContext>(m, "ApplicationContext")
      .def(py::init())
      .def("detach", &squish::ApplicationContext::Detach)
      .def_property_readonly("isRunning", &squish::ApplicationContext::IsRunning)
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

  py::class_<squish::Object>(m, "object")
      .def(py::init())
      .def(py::init<int, int>(), "", py::arg("x"), py::arg("y"))
      .def(py::init<int, int, int, int>(), "", py::arg("x"), py::arg("y"), py::arg("width"), py::arg("height"))
      .def("exists", &squish::Object::Exists,
           "This function returns a true value if the object with the symbolic or real (multi-property) "
           "name objectName exists; otherwise it returns a false value.",
           py::arg("objectName"))
      .def_readwrite("x", &squish::Object::x)
      .def_readwrite("y", &squish::Object::y)
      .def_readwrite("width", &squish::Object::width)
      .def_readwrite("height", &squish::Object::height)
      .def_readwrite("name", &squish::Object::name)
      .def_readwrite("type", &squish::Object::type);

  py::register_exception<squish::LookupError>(m, "LookupError");
  py::register_exception<squish::InvalidSyncVersion>(m, "InvalidSyncVersion");
  py::register_exception<squish::InvalidSyncBuildVersion>(m, "InvalidSyncBuildVersion");
  py::register_exception<squish::InvalidSyncCollectionMode>(m, "InvalidSyncCollectionMode");
  py::register_exception<squish::InvalidDurationLongPress>(m, "InvalidDurationLongPress");
  py::register_exception<interaction::VideoStreamingError>(m, "VideoStreamingError");
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

  m.def("touchAndDrag",
        py::overload_cast<const squish::Object&, int, int, int, int, common::squish::ModifierState>(
            &squish::API::TouchAndDrag),
        "TouchAndDrag performs a touch-based drag operation. It initiates a touch drag of the specified objectOrName"
        "widget starting at position x, y and is dragged by dx pixels horizontally and by dy pixels vertically.",
        py::arg("object_or_name"), py::arg("x"), py::arg("y"), py::arg("dx"), py::arg("dy"), py::arg("modifier"));

  m.def("touchAndDrag",
        py::overload_cast<const std::string&, int, int, int, int, common::squish::ModifierState>(
            &squish::API::TouchAndDrag),
        "TouchAndDrag performs a touch-based drag operation. It initiates a touch drag of the specified objectOrName"
        "widget starting at position x, y and is dragged by dx pixels horizontally and by dy pixels vertically.",
        py::arg("object_or_name"), py::arg("x"), py::arg("y"), py::arg("dx"), py::arg("dy"), py::arg("modifier"));

  m.def("touchAndDrag", py::overload_cast<const squish::Object&, int, int, int, int>(&squish::API::TouchAndDrag),
        "TouchAndDrag performs a touch-based drag operation. It initiates a touch drag of the specified objectOrName"
        "widget starting at position x, y and is dragged by dx pixels horizontally and by dy pixels vertically.",
        py::arg("object_or_name"), py::arg("x"), py::arg("y"), py::arg("dx"), py::arg("dy"));

  m.def("touchAndDrag", py::overload_cast<const std::string&, int, int, int, int>(&squish::API::TouchAndDrag),
        "TouchAndDrag performs a touch-based drag operation. It initiates a touch drag of the specified objectOrName"
        "widget starting at position x, y and is dragged by dx pixels horizontally and by dy pixels vertically.",
        py::arg("object_or_name"), py::arg("x"), py::arg("y"), py::arg("dx"), py::arg("dy"));

  m.def("changeSyncIconDB", &squish::API::ChangeSyncIconDB,
        "ChangeSyncIconDB performs a change sync_version and sync build_version for getting search item from a specific"
        "collection.",
        py::arg("sync_version"), py::arg("sync_build_version"));

  py::enum_<common::squish::CollectionMode>(m, "CollectionMode")
      .value("DAY", common::squish::CollectionMode::DAY)
      .value("NIGHT", common::squish::CollectionMode::NIGHT);

  m.def("changeSyncMode", &squish::API::ChangeSyncMode, "This function changes active collection mode in DBManager ",
        py::arg("collection_mode"));

  m.def(
      "longPress", py::overload_cast<const squish::Object&, int>(&squish::API::LongPress),
      "This function is pressing on the specified object a pointed amount of milliseconds, or 2 seconds if timeout not "
      "specified. Throws 'InvalidDurationLongPress' in case if timeout_msec longer than 60 seconds.",
      py::arg("object_or_name"), py::arg("timeout_msec") = int(kDefaultLongPressTimeout));

  m.def("longPress", py::overload_cast<const squish::Object&, int, int, int>(&squish::API::LongPress),
        "This function is taps by pressing with specified timeout in milliseconds on the specified object. "
        "The x, y coordinates and timeout_msec are optional. If they are"
        "not specified the tap is made in the center of the widget. On the other hand, if the additional parameters are"
        "given, the tap is made at position x and y (in the object coordinates). Throws 'InvalidDurationLongPress' in "
        "case if timeout_msec longer than 60 seconds.",
        py::arg("object_or_name"), py::arg("x"), py::arg("y"),
        py::arg("timeout_msec") = int(kDefaultLongPressTimeout));
}