cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# A separate static library is necessary to support unit tests
add_library(vhat_server_lib
  STATIC
    config/config.cpp
    storage/json_storage.cpp
    ate.cpp
)

target_include_directories(vhat_server_lib
  PUBLIC
    ../include
    "${OpenCV_INCLUDE_DIRS}"
)

# JsonCpp
# http://open-source-parsers.github.io/jsoncpp-docs/doxygen/index.html
# Debian/Ubuntu package name: libjsoncpp-dev
find_package(jsoncpp REQUIRED)

# Get OpenCV version from config
file(STRINGS "${CMAKE_SOURCE_DIR}/infrastructure/config/opencv-version" VHAT_OPENCV_VERSION
  LIMIT_COUNT 1
)

if (NOT DEFINED VHAT_OPENCV_VERSION)
message(FATAL_ERROR "VHAT_OPENCV_VERSION is not defined")
endif(NOT DEFINED VHAT_OPENCV_VERSION)

find_package(OpenCV "${VHAT_OPENCV_VERSION}" EXACT REQUIRED
  COMPONENTS
    calib3d
    core
    features2d
    imgcodecs
    imgproc
    videoio
)

target_link_libraries(vhat_server_lib
  inihcpp
  jsoncpp_lib
  Boost::asio
  opencv_calib3d
  opencv_core
  opencv_features2d
  opencv_imgcodecs
  opencv_imgproc
  opencv_videoio
)

add_executable(vhat_server
  ate_server_main.cpp
)

target_compile_options(vhat_server_lib
  PUBLIC -Wall -Wextra -pedantic -Werror)

target_link_libraries(vhat_server
  PRIVATE
    vhat_server_lib
)
