cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# A separate static library is necessary to support unit tests
add_library(vhat_server_lib
  STATIC
    config/config.cpp
    interaction/vdp_interaction.cpp
    interaction/https_client.cpp
    storage/json_storage.cpp
    utils/logger_setup.cpp
    video_streaming/stream_reader.cpp
    video_streaming/matching/template_detector.cpp
    ate.cpp
)

target_include_directories(vhat_server_lib
  PUBLIC
    ../include
    "${OpenCV_INCLUDE_DIRS}"
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
  vhat_common
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

target_compile_definitions(vhat_server_lib
  PUBLIC
    VHAT_DATA_PATH="${VHAT_DATA_PATH}"
)

target_link_libraries(vhat_server
  PRIVATE
    vhat_server_lib
)
