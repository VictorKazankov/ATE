cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# A separate static library is necessary to support unit tests
add_library(vhat_server_lib
  STATIC
    interaction/dummy/dummy_interaction.cpp
    interaction/VDP/vdp_interaction.cpp
    interaction/VDP/https_client.cpp
    interaction/ATE/tcp_connection.cpp
    interaction/ATE/tcp_server.cpp
    interaction/ATE/tcp_session_handler.cpp
    interaction/SPI/spi_interaction.cpp
    storage/json_storage.cpp
    utils/safe_env.cpp
    video_streaming/matching/matcher.cpp
    video_streaming/matching/template_detector.cpp
    video_streaming/matching/text_detector.cpp
    video_streaming/matching/text_detector_exceptions.cpp
    video_streaming/stream_reader.cpp
    ate.cpp
)

# OpenCV
# https://opencv.org
find_package(OpenCV 3.3.1 REQUIRED
  COMPONENTS
    calib3d
    core
    features2d
    imgcodecs
    imgproc
    videoio
)

# Leptonica by Dan Bloomberg
# http://www.leptonica.org
find_package(Leptonica REQUIRED)

# Tesseract
# https://github.com/tesseract-ocr/tesseract
find_package(Tesseract REQUIRED)

target_include_directories(vhat_server_lib
  PUBLIC
    ../include
    "${Leptonica_INCLUDE_DIRS}"
    "${OpenCV_INCLUDE_DIRS}"
    "${Tesseract_INCLUDE_DIRS}"
)

target_link_libraries(vhat_server_lib
  vhat_common
  stdc++fs
  opencv_calib3d
  opencv_core
  opencv_features2d
  opencv_imgcodecs
  opencv_imgproc
  opencv_videoio
  ${Leptonica_LIBRARIES}
  ${Tesseract_LIBRARIES}
)

add_executable(vhat_server
  ate_server_main.cpp
)

target_compile_definitions(vhat_server_lib
  PUBLIC
    VHAT_SERVER_DATA_PATH="${VHAT_SERVER_DATA_PATH}"
)

target_link_libraries(vhat_server
  PRIVATE
    vhat_server_lib
)
