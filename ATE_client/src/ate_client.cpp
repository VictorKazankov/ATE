#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <boost/system/system_error.hpp>

#include "api.h"
#include "error_defines.h"
#include "message_factory/json_defines.h"
#include "utils/geometry_types.h"
#include "utils/squish_types.h"

namespace py = pybind11;
using namespace common::jmsg;

namespace {
const uint32_t kDefaultLongPressTimeout = 2000;
}  // namespace

PYBIND11_MODULE(vhat_client, m) {
  m.doc() = "ATE plugin for communication with SYNC";

  py::class_<squish::ApplicationContext>(m, "ApplicationContext")
      .def(py::init())
      .def("detach", &squish::ApplicationContext::Detach)
      .def_property_readonly("isRunning", &squish::ApplicationContext::IsRunning)
      .def_property_readonly(kHost, &squish::ApplicationContext::host)
      .def_property_readonly(kPort, &squish::ApplicationContext::port);

  py::class_<common::Point>(m, "ScreenPoint")
      .def(py::init())
      .def(py::init<int&, int&>(), "", py::arg(kAbscissa), py::arg(kOrdinate))
      .def_readwrite(kAbscissa, &common::Point::x)
      .def_readwrite(kOrdinate, &common::Point::y);

  py::class_<common::Rect>(m, "ScreenRectangle")
      .def(py::init())
      .def(py::init<int&, int&, int&, int&>(), "", py::arg(kAbscissa), py::arg(kOrdinate), py::arg(kWidth),
           py::arg(kHeight))
      .def_readwrite(kAbscissa, &common::Rect::x)
      .def_readwrite(kOrdinate, &common::Rect::y)
      .def_readwrite(kWidth, &common::Rect::width)
      .def_readwrite(kHeight, &common::Rect::height);

  py::class_<squish::Object>(m, "object")
      .def(py::init())
      .def(py::init<int, int>(), "", py::arg(kAbscissa), py::arg(kOrdinate))
      .def(py::init<int, int, int, int>(), "", py::arg(kAbscissa), py::arg(kOrdinate), py::arg(kWidth),
           py::arg(kHeight))
      .def("exists", [](const squish::Object&, std::string& object_name) { return API::Exists(object_name); },
           "This function returns a true value if the object with the symbolic or real (multi-property) "
           "name objectName exists; otherwise it returns a false value."
           "Throws:"
           " 'NoConnectionEstablished' in case of no connection was established to server-side"
           " 'VideoStreamNotFound' in case of the video stream is not available"
           " 'invalid_argument' in case of invalid params sent to the server-side"
           " 'runtime_error' in case of internal error, parse error, invalid request, method not found",
           py::arg("objectName"))
      .def_property_readonly("topLeft", &squish::Object::TopLeft)
      .def_property_readonly("bottomRight", &squish::Object::BottomRight)
      .def_readwrite(kAbscissa, &squish::Object::x)
      .def_readwrite(kOrdinate, &squish::Object::y)
      .def_readwrite(kWidth, &squish::Object::width)
      .def_readwrite(kHeight, &squish::Object::height)
      .def_readwrite(kXTopLeft, &squish::Object::x_top_left)
      .def_readwrite(kYTopLeft, &squish::Object::y_top_left)
      .def_readwrite(kXBottomRight, &squish::Object::x_bottom_right)
      .def_readwrite(kYBottomRight, &squish::Object::y_bottom_right)
      .def_readwrite(kParentWidth, &squish::Object::parent_width)
      .def_readwrite(kParentHeight, &squish::Object::parent_height)
      .def_readwrite(kSyncVersion, &squish::Object::sync_version)
      .def_readwrite(kSyncBuildVersion, &squish::Object::build_version)
      .def_readwrite(kSyncCollectionMode, &squish::Object::mode)
      .def_readwrite(kName, &squish::Object::name)
      .def_readwrite(kParentScreen, &squish::Object::parent_screen);

  py::class_<squish::Wildcard>(m, "Wildcard")
      .def("__init__",
           [](squish::Wildcard& instance, py::dict& dict) {
             std::string name;
             std::string sync_version;
             std::string build_version;
             std::string parent_name;
             common::squish::CollectionMode mode = common::squish::CollectionMode::kNone;

             if (dict.contains(kName)) {
               auto name_value = dict[common::jmsg::kName];
               if (py::isinstance<py::str>(name_value)) {
                 name = name_value.cast<std::string>();
               }
             }

             if (dict.contains(kSyncVersion)) {
               auto sync_version_value = dict[common::jmsg::kSyncVersion];
               if (py::isinstance<py::str>(sync_version_value)) {
                 sync_version = sync_version_value.cast<std::string>();
               }
             }

             if (dict.contains(kSyncBuildVersion)) {
               auto build_version_value = dict[common::jmsg::kSyncBuildVersion];
               if (py::isinstance<py::str>(build_version_value)) {
                 build_version = build_version_value.cast<std::string>();
               }
             }

             if (dict.contains(kParentScreen)) {
               auto parent_name_value = dict[common::jmsg::kParentScreen];
               if (py::isinstance<py::str>(parent_name_value)) {
                 parent_name = parent_name_value.cast<std::string>();
               }
             }

             if (dict.contains(kSyncCollectionMode)) {
               auto mode_value = dict[common::jmsg::kSyncCollectionMode];
               if (py::isinstance<common::squish::CollectionMode>(mode_value)) {
                 mode = mode_value.cast<common::squish::CollectionMode>();
               }
             }

             new (&instance) squish::Wildcard(name, sync_version, build_version, parent_name, mode);
           },
           "Throws:"
           " 'NoConnectionEstablished' in case of no connection was established to server-side"
           " 'invalid_argument' in case of class was filled by invalid params"
           " 'runtime_error' in case of internal error, parse error, etc.")
      .def_readwrite(kName, &squish::Wildcard::name)
      .def_readwrite(kSyncVersion, &squish::Wildcard::sync_version)
      .def_readwrite(kSyncBuildVersion, &squish::Wildcard::build_version)
      .def_readwrite(kParentScreen, &squish::Wildcard::parent_screen)
      .def_readwrite(kSyncCollectionMode, &squish::Wildcard::mode)
      .def("getMatchObjects", &squish::Wildcard::GetMatchObjects,
           "This function returns results that matches to defined criterion, "
           "the non-empty list of the 'squish::objects' if pattern matches to any object in the database, "
           "otherwise empty list");

  py::register_exception<squish::LookupError>(m, "LookupError");
  py::register_exception<squish::InvalidSyncVersion>(m, "InvalidSyncVersion");
  py::register_exception<squish::InvalidSyncBuildVersion>(m, "InvalidSyncBuildVersion");
  py::register_exception<squish::InvalidSyncCollectionMode>(m, "InvalidSyncCollectionMode");
  py::register_exception<squish::InvalidDurationLongPress>(m, "InvalidDurationLongPress");
  py::register_exception<squish::EmptyScreenshotFileName>(m, "EmptyScreenshotFileName");
  py::register_exception<squish::WrongScreenshotExtension>(m, "WrongScreenshotExtension");
  py::register_exception<squish::PermissionDenied>(m, "PermissionDenied");
  py::register_exception<squish::ImageAssemblingFailed>(m, "ImageAssemblingFailed");
  py::register_exception<squish::InvalidRectangleCoordinates>(m, "InvalidRectangleCoordinates");
  py::register_exception<squish::ComparingImageIncorrectSize>(m, "ComparingImageIncorrectSize");
  py::register_exception<squish::ComparingImageNotExist>(m, "ComparingImageNotExist");
  py::register_exception<squish::UnsupportedFileType>(m, "UnsupportedFileType");
  py::register_exception<interaction::VideoStreamingError>(m, "VideoStreamingError");
  py::register_exception<interaction::NoConnectionEstablished>(m, "NoConnectionEstablished");
  py::register_exception<boost::system::system_error>(m, "boost_system_error");

  py::enum_<common::squish::CollectionMode>(m, "CollectionMode")
      .value("NONE", common::squish::CollectionMode::kNone)
      .value("ANY", common::squish::CollectionMode::kAny)
      .value("DAY", common::squish::CollectionMode::kDay)
      .value("NIGHT", common::squish::CollectionMode::kNight);

  py::enum_<common::squish::ModifierState>(m, "ModifierState")
      .value("NONE", common::squish::ModifierState::kNone)
      .value("ALT", common::squish::ModifierState::kAlt)
      .value("CONTROL", common::squish::ModifierState::kControl)
      .value("SHIFT", common::squish::ModifierState::kShift);

  py::enum_<common::squish::MouseButton>(m, "MouseButton")
      .value("NONE", common::squish::MouseButton::kNone)
      .value("LEFT_BUTTON", common::squish::MouseButton::kLeftButton)
      .value("MIDDLE_BUTTON", common::squish::MouseButton::kMiddleButton)
      .value("RIGHT_BUTTON", common::squish::MouseButton::kRightButton);

  m.def("attachToApplication", &API::AttachToApplication,
        "This function causes to attach to the application called aut_name and returns a handle to its application "
        "context. ",
        py::arg("aut_name"), py::return_value_policy::reference);

  m.def("waitForObject", py::overload_cast<const std::string&>(&API::WaitForObject),
        "waitForObject waits until the objectOrName object is accessible (i.e., it exists and is visible and enabled). "
        "The function waits for the time defined by the testSettings.waitForObjectTimeout property, that many "
        "milliseconds. "
        "Throws:"
        " 'LookupError' in case of the pattern is not detected on the screen or timeout has expired"
        " 'VideoStreamingError' in case of the video stream is not available"
        " 'NoConnectionEstablished' in case of no connection was established to server-side"
        " 'invalid_argument' in case of the invalid arguments in request"
        " 'runtime_error' in case of an internal error, parse error, invalid request, a method not found",
        py::arg("object_or_name"));

  m.def("waitForObject", py::overload_cast<const squish::Object&>(&API::WaitForObject),
        "waitForObject waits until the objectOrName object is accessible (i.e., it exists and is visible and enabled). "
        "The function waits for the time defined by the testSettings.waitForObjectTimeout property, that many "
        "milliseconds. "
        "Throws:"
        " 'LookupError' in case of the pattern is not detected on the screen or timeout has expired"
        " 'VideoStreamingError' in case of the video stream is not available"
        " 'NoConnectionEstablished' in case of no connection was established to server-side"
        " 'invalid_argument' in case of the invalid arguments in request"
        " 'runtime_error' in case of an internal error, parse error, invalid request, a method not found",
        py::arg("object_or_name"));

  m.def("waitForObject", py::overload_cast<const squish::Wildcard&>(&API::WaitForObject),
        "waitForObject waits until the objectOrName object is accessible (i.e., it exists and is visible and enabled) "
        "and returns the first found object. "
        "The function waits for the time defined by the testSettings.waitForObjectTimeout property, that many "
        "milliseconds. "
        "Throws:"
        " 'LookupError' in case of the pattern is not detected on the screen or timeout has expired"
        " 'VideoStreamingError' in case of the video stream is not available"
        " 'NoConnectionEstablished' in case of no connection was established to server-side"
        " 'invalid_argument' in case of the invalid arguments in request"
        " 'runtime_error' in case of an internal error, parse error, invalid request, a method not found",
        py::arg("wildcard"));

  m.def("waitForObject", py::overload_cast<const squish::Wildcard&, int>(&API::WaitForObject),
        "waitForObject waits until the objectOrName object is accessible (i.e., it exists and is visible and enabled) "
        "and returns the first found object. "
        "The function waits for the time defined by the optional timeoutMSec parameter is used, that many "
        "milliseconds. This function is useful if you want to synchronize your script execution. "
        "Throws:"
        " 'LookupError' in case of the pattern is not detected on the screen or timeout has expired"
        " 'VideoStreamingError' in case of the video stream is not available"
        " 'NoConnectionEstablished' in case of no connection was established to server-side"
        " 'invalid_argument' in case of the invalid arguments in request"
        " 'runtime_error' in case of an internal error, parse error, invalid request, a method not found",
        py::arg("wildcard"), py::arg("timeout_msec"));

  m.def("waitForObject", py::overload_cast<const std::string&, int>(&API::WaitForObject),
        "waitForObject waits until the objectOrName object is accessible (i.e., it exists and is visible and enabled). "
        "The function waits for the time defined by the optional timeoutMSec parameter is used, that many "
        "milliseconds. This function is useful if you want to synchronize your script execution. "
        "Throws:"
        " 'LookupError' in case of the pattern is not detected on the screen or timeout has expired"
        " 'VideoStreamingError' in case of the video stream is not available"
        " 'NoConnectionEstablished' in case of no connection was established to server-side"
        " 'invalid_argument' in case of the invalid arguments in request"
        " 'runtime_error' in case of an internal error, parse error, invalid request, a method not found",
        py::arg("object_or_name"), py::arg("timeout_msec"));

  m.def("waitForObject", py::overload_cast<const squish::Object&, int>(&API::WaitForObject),
        "waitForObject waits until the objectOrName object is accessible (i.e., it exists and is visible and enabled). "
        "The function waits for the time defined by the optional timeoutMSec parameter is used, that many "
        "milliseconds. This function is useful if you want to synchronize your script execution. "
        "Throws:"
        " 'LookupError' in case of the pattern is not detected on the screen or timeout has expired"
        " 'VideoStreamingError' in case of the video stream is not available"
        " 'NoConnectionEstablished' in case of no connection was established to server-side"
        " 'invalid_argument' in case of the invalid arguments in request"
        " 'runtime_error' in case of an internal error, parse error, invalid request, a method not found",
        py::arg("object_or_name"), py::arg("timeout_msec"));

  m.def("tapObject",
        py::overload_cast<const common::Point&, common::squish::ModifierState, common::squish::MouseButton>(
            &API::TapObject),
        "tapObject performs a touch tap at the position specified by screenPoint. Position are in screen global "
        "coordinates. "
        "Throws:"
        " 'runtime_error' in case system's erorrs"
        " 'NoConnectionEstablished' in case of no connection was established to server-side"
        " 'invalid_argument' in case of invalid params sent to the server-side",
        py::arg("screen_point"), py::arg("modifier_state"), py::arg("button"));

  m.def("tapObject",
        py::overload_cast<const common::Rect&, common::squish::ModifierState, common::squish::MouseButton>(
            &API::TapObject),
        "tapObject performs a touch tap at the center of the rectangle specified by screenRectangle. Position are in "
        "screen global coordinates. "
        "Throws:"
        " 'runtime_error' in case system's erorrs"
        " 'NoConnectionEstablished' in case of no connection was established to server-side"
        " 'invalid_argument' in case of invalid params sent to the server-side",
        py::arg("screen_rectangle"), py::arg("modifier_state"), py::arg("button"));

  m.def("tapObject",
        py::overload_cast<const squish::Object&, common::squish::ModifierState, common::squish::MouseButton>(
            &API::TapObject),
        "tapObject performs a touch tap at the center of the rectangle specified by screenRectangle. Position are in "
        "screen global coordinates. "
        "Throws:"
        " 'runtime_error' in case system's erorrs"
        " 'NoConnectionEstablished' in case of no connection was established to server-side"
        " 'invalid_argument' in case of invalid params sent to the server-side",
        py::arg("screen_rectangle"), py::arg("modifier_state"), py::arg("button"));

  m.def("touchAndDrag",
        py::overload_cast<const squish::Object&, int, int, int, int, common::squish::ModifierState>(&API::TouchAndDrag),
        "TouchAndDrag performs a touch-based drag operation. It initiates a touch drag of the specified objectOrName"
        "widget starting at position x, y and is dragged by dx pixels horizontally and by dy pixels vertically. "
        "Throws:"
        " 'runtime_error' in case system's erorrs"
        " 'NoConnectionEstablished' in case of no connection was established to server-side"
        " 'invalid_argument' in case of invalid params sent to the server-side",
        py::arg("object_or_name"), py::arg("x"), py::arg("y"), py::arg("dx"), py::arg("dy"),
        py::arg("modifier") = common::squish::ModifierState::kNone);

  m.def("touchAndDrag",
        py::overload_cast<const std::string&, int, int, int, int, common::squish::ModifierState>(&API::TouchAndDrag),
        "TouchAndDrag performs a touch-based drag operation. It initiates a touch drag of the specified objectOrName"
        "widget starting at position x, y and is dragged by dx pixels horizontally and by dy pixels vertically. "
        "Throws:"
        " 'runtime_error' in case system's errors"
        " 'NoConnectionEstablished' in case of no connection was established to server-side"
        " 'invalid_argument' in case of invalid params sent to the server-side",
        py::arg("object_or_name"), py::arg("x"), py::arg("y"), py::arg("dx"), py::arg("dy"),
        py::arg("modifier") = common::squish::ModifierState::kNone);

  m.def("pressAndHold", py::overload_cast<const common::Point&>(&API::PressAndHold),
        "pressAndHold performs a press and hold operation specified by a screen_point. Position is in screen global "
        "coordinates. Press and hold operation doesn't have a timeout for execution and can be interrupted by "
        "pressRelease() API only. Any subsequent call of any other touch based operation prior calling pressRelease() "
        "will cause undefined behaviour"
        "Throws:"
        " 'NoConnectionEstablished' in case of no connection was established to server-side"
        " 'runtime_error' in case system's errors"
        " 'invalid_argument' in case of invalid params sent to the server-side",
        py::arg("screen_point"));

  m.def("pressAndHold", py::overload_cast<const common::Rect&>(&API::PressAndHold),
        "pressAndHold performs a press and hold operation specified by the center of a screen_rectangle. Position is "
        "in screen global coordinates. Press and hold operation doesn't have a timeout for execution and can be "
        "interrupted by pressRelease() API only. Any subsequent call of any other touch based operation prior calling "
        "pressRelease() will cause undefined behaviour"
        "Throws:"
        " 'NoConnectionEstablished' in case of no connection was established to server-side"
        " 'runtime_error' in case system's errors"
        " 'invalid_argument' in case of invalid params sent to the server-side",
        py::arg("screen_rectangle"));

  m.def("pressAndHold", py::overload_cast<const squish::Object&>(&API::PressAndHold),
        "pressAndHold performs a press and hold operation specified by the center of the object obtained by "
        "waitForObject(). Press and hold operation doesn't have a timeout for execution and can be interrupted by "
        "pressRelease() API only. Any subsequent call of any other touch based operation prior calling pressRelease() "
        "will cause undefined behaviour"
        "Throws:"
        " 'NoConnectionEstablished' in case of no connection was established to server-side"
        " 'runtime_error' in case system's errors"
        " 'invalid_argument' in case of invalid params sent to the server-side",
        py::arg("object"));

  m.def("pressRelease", py::overload_cast<const common::Point&>(&API::PressRelease),
        "This function performs a release operation to interrupt running pressAndHold() API at the position specified "
        "by screen_point in screen global coordinates. Passing argument other than in preceding pressAndHold() call "
        "will cause undefined behaviour."
        "Throws:"
        " 'NoConnectionEstablished' in case of no connection was established to server-side"
        " 'runtime_error' in case system's errors"
        " 'invalid_argument' in case of invalid params sent to the server-side",
        py::arg("screen_point"));

  m.def("pressRelease", py::overload_cast<const common::Rect&>(&API::PressRelease),
        "This function performs a release operation to interrupt running pressAndHold() API at the position specified "
        "by the center of the screen_rectangle in screen global coordinates. Passing argument other than in preceding "
        "pressAndHold() call will cause undefined behaviour."
        "Throws:"
        " 'NoConnectionEstablished' in case of no connection was established to server-side"
        " 'runtime_error' in case system's errors"
        " 'invalid_argument' in case of invalid params sent to the server-side",
        py::arg("screen_rectangle"));

  m.def("pressRelease", py::overload_cast<const squish::Object&>(&API::PressRelease),
        "This function performs a release operation to interrupt running pressAndHold() API at the position specified "
        "by object provided as a result of waitFordObject() API. Passing argument other than in preceding "
        "pressAndHold() call will cause undefined behaviour."
        "Throws:"
        " 'NoConnectionEstablished' in case of no connection was established to server-side"
        " 'runtime_error' in case system's errors"
        " 'invalid_argument' in case of invalid params sent to the server-side",
        py::arg("object"));

  m.def("changeSyncIconDB", &API::ChangeSyncIconDB,
        "ChangeSyncIconDB performs a change sync_version and sync build_version for getting search item from a "
        "specific collection. "
        "Throws:"
        " 'InvalidSyncVersion' In case of the sync_version is incorrect."
        " 'InvalidSyncBuildVersion' In case of the sync_build_version is incorrect."
        " 'runtime_error' in case system's erorrs"
        " 'NoConnectionEstablished' in case of no connection was established to server-side"
        " 'invalid_argument' in case of invalid params sent to the server-side",
        py::arg("sync_version"), py::arg("sync_build_version"));

  m.def("changeSyncMode", &API::ChangeSyncMode,
        "This function changes active collection mode in DBManager. "
        "Throws:"
        " 'InvalidSyncCollectionMode' In case of the collection_mode is incorrect"
        " 'runtime_error' in case system's erorrs"
        " 'NoConnectionEstablished' in case of no connection was established to server-side"
        " 'invalid_argument' in case of invalid params sent to the server-side",
        py::arg("collection_mode"));

  m.def(
      "longPress", py::overload_cast<const squish::Object&, int>(&API::LongPress),
      "This function is pressing on the specified object a pointed amount of milliseconds, or 2 seconds if timeout not "
      "specified. Throws 'InvalidDurationLongPress' in case if timeout_msec longer than 60 seconds."
      "Throws:"
      " 'NoConnectionEstablished' in case of no connection was established to server-side"
      " 'runtime_error' in case system's errors"
      " 'invalid_argument' in case of invalid params sent to the server-side",
      py::arg("object_or_name"), py::arg("timeout_msec") = int(kDefaultLongPressTimeout));

  m.def("longPress", py::overload_cast<const squish::Object&, int, int, int>(&API::LongPress),
        "This function is taps by pressing with specified timeout in milliseconds on the specified object. "
        "The x, y coordinates and timeout_msec are optional. If they are"
        "not specified the tap is made in the center of the widget. On the other hand, if the additional parameters are"
        "given, the tap is made at position x and y (in the object coordinates). Throws 'InvalidDurationLongPress' in "
        "case if timeout_msec longer than 60 seconds."
        "Throws:"
        " 'NoConnectionEstablished' in case of no connection was established to server-side"
        " 'runtime_error' in case system's errors"
        " 'invalid_argument' in case of invalid params sent to the server-side",
        py::arg("object_or_name"), py::arg("x"), py::arg("y"), py::arg("timeout_msec") = int(kDefaultLongPressTimeout));

  m.def("getScreenshot", py::overload_cast<const std::string&, const std::string&>(&API::GetScreenshot),
        "This function captures screenshot from connected sync screen and save it inside declared directory."
        "filename - the name of screenshot file must have 'png' extension"
        "location - directory name for storing screenshot with prefix '/var/lib/vdp/vhat'"
        "Returns: true in case of success"
        "Throws:"
        " 'VideoStreamNotFound' in case of the video stream is not available"
        " 'EmptyScreenshotFileName' in case of passed name of the screenshot is empty"
        " 'WrongScreenshotExtension' in case of screenshot extansion is not 'png'"
        " 'PermissionDenied' in case of server does not have permission to make directory"
        " 'ImageAssemblingFailed' in case of server can't save the screenshot"
        " 'NoConnectionEstablished' in case of no connection was established to server-side"
        " 'runtime_error' in case system's errors"
        " 'invalid_argument' in case of invalid params sent to the server-side",
        py::arg("filename"), py::arg("location"));

  m.def("getText", &API::GetText,
        "GetText returns the text by specified coordinates. "
        "The x1, y1 - topleft coordinate. "
        "The x2, y2 - bottom-right coordinate. "
        "Throws:"
        " 'InvalidRectangleCoordinates' in case top-left and bottom-right coordinates are mixed up or "
        "                               produced rectangle has zero height/width or is out of frame boundaries."
        " 'NoConnectionEstablished' in case of no connection was established to server-side"
        " 'runtime_error' in case system's errors"
        " 'invalid_argument' in case of invalid params sent to the server-side"
        " 'VideoStreamNotFound' in case of the video stream is not available",
        py::arg("x1"), py::arg("y1"), py::arg("x2"), py::arg("y2"), py::return_value_policy::copy);

  m.def("getObjectsDataByPattern", py::overload_cast<const squish::Wildcard&>(&API::GetObjectsDataByPattern),
        "GetObjectsDataByPattern provides a list of objects that are formed on the basis of the available data in the "
        "database. The selection of objects is carried out by applying a search pattern."
        "Throws:"
        " 'runtime_error' in case system's erorrs"
        " 'NoConnectionEstablished' in case of no connection was established to server-side"
        " 'invalid_argument' in case of invalid params sent to the server-side",
        py::arg("wildcard"));

  m.def("getObjectsDataByPattern", py::overload_cast<const std::string&>(&API::GetObjectsDataByPattern),
        "GetObjectsDataByPattern provides a list of objects that are formed on the basis of the available data in the "
        "database. The selection of objects is carried out by applying a search pattern."
        "Throws:"
        " 'runtime_error' in case system's erorrs"
        " 'NoConnectionEstablished' in case of no connection was established to server-side"
        " 'invalid_argument' in case of invalid params sent to the server-side",
        py::arg("name"));

  m.def("getImagesDiscrepancy", &API::GetImagesDiscrepancy,
        "GetImagesDiscrepancy provides functionality for comparing two images and returns the percentage of "
        "      discrepancy. If you need to compare certain areas of the image, specify the coordinates of the top left "
        "      corner (point_top_left) and bottom right (point_bottom_right)."
        "Throws:"
        " 'InvalidRectangleCoordinates' In case top-left and bottom-right coordinates are mixed up or "
        "                               produced rectangle has zero height/width or is out of frame boundaries."
        " 'ComparingImageIncorrectSize' In case incorrect size of image"
        " 'ComparingImageNotExist' In case comparing images doesn't exist"
        " 'EmptyScreenshotFileName' In case comparing image file path is empty"
        " 'PermissionDenied' In case error read permissions"
        " 'runtime_error' In case system's errors"
        " 'NoConnectionEstablished' In case of no connection was established to server-side"
        " 'invalid_argument' In case of invalid params sent to the server-side"
        " 'UnsupportedFileType' In case of incorrect image file type",
        py::arg("icon_path_second"), py::arg("icon_path_first"),
        py::arg_v("point_top_left", common::Point(), "The top left coordinate of the comparison area"),
        py::arg_v("point_bottom_right", common::Point(), "The bottom right coordinate of the comparison area"));
}
