cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

include(GNUInstallDirs)

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
    utils/file_descriptor.cpp
    utils/screenshot_recorder.cpp
    video_streaming/matching/matcher.cpp
    video_streaming/matching/multiscale_template_detector.cpp
    video_streaming/matching/template_detector.cpp
    video_streaming/matching/text_detector.cpp
    video_streaming/matching/text_detector_exceptions.cpp
    video_streaming/matching/text_detector_decorator.cpp
    video_streaming/v4l_streamer.cpp
    video_streaming/framebuf_to_mat.cpp
    video_streaming/gst_streamer.cpp
    video_streaming/stream_reader.cpp
    video_streaming/streamer_factory.cpp
    ate.cpp
)

# pkg-config
# https://cmake.org/cmake/help/latest/module/FindPkgConfig.html
# https://www.freedesktop.org/wiki/Software/pkg-config
find_package(PkgConfig REQUIRED)

# GStreamer
# https://gstreamer.freedesktop.org
pkg_check_modules(GStreamer REQUIRED
  gstreamer-1.0
  gstreamer-app-1.0
)

# OpenCV
# https://opencv.org
find_package(OpenCV 3.3.1 REQUIRED
  COMPONENTS
    core
    imgcodecs
    imgproc
    videoio
)

# Leptonica by Dan Bloomberg
# http://www.leptonica.org
find_package(Leptonica 1.74 REQUIRED)

# Tesseract
# https://github.com/tesseract-ocr/tesseract
set(MINIMUM_TESSERACT_VERSION 4)
find_package(Tesseract ${MINIMUM_TESSERACT_VERSION} QUIET)
if(Tesseract_FOUND)
  message(STATUS "Using CMake find_package for finding Tesseract")
else(Tesseract_FOUND)
  message(STATUS "Using pkg-config for finding Tesseract")
  pkg_check_modules(Tesseract REQUIRED
    tesseract>=${MINIMUM_TESSERACT_VERSION}
  )
endif(Tesseract_FOUND)

# Workaround for build project while some packages found with pkg-config
# TODO: Use IMPORTED_TARGET for pkg_check_modules since CMake 3.6
link_directories(
  "${GStreamer_LIBRARY_DIRS}"
  "${Tesseract_LIBRARY_DIRS}"
)

target_include_directories(vhat_server_lib
  PUBLIC
    ../include
    "${GStreamer_INCLUDE_DIRS}"
    "${Leptonica_INCLUDE_DIRS}"
    "${OpenCV_INCLUDE_DIRS}"
    "${Tesseract_INCLUDE_DIRS}"
)

target_link_libraries(vhat_server_lib
  PUBLIC
    vhat_common
    boost_asio_ssl
    stdc++fs
    opencv_core
    opencv_imgcodecs
    opencv_imgproc
    opencv_videoio
    ${GStreamer_LIBRARIES}
    ${Leptonica_LIBRARIES}
    ${Tesseract_LIBRARIES}
)

add_executable(vhat_server
  ate_server_main.cpp
)

set(VHAT_SERVER_STORAGE_SUBDIR "${CMAKE_INSTALL_DATADIR}/vhat_server")

target_compile_definitions(vhat_server_lib
  PUBLIC
    VHAT_SERVER_CONFIG_DIR="${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_SYSCONFDIR}"
    VHAT_SERVER_STORAGE_DIR="${CMAKE_INSTALL_PREFIX}/${VHAT_SERVER_STORAGE_SUBDIR}"
)

target_link_libraries(vhat_server
  PRIVATE
    vhat_server_lib
)

set_target_properties(vhat_server PROPERTIES
  INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}"
  INSTALL_RPATH_USE_LINK_PATH TRUE
)

install(FILES
  ../data/vhat_server.ini
  DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}
)

install(DIRECTORY
  ../data/icon_storage
  DESTINATION "${VHAT_SERVER_STORAGE_SUBDIR}"
)

install(TARGETS
  vhat_server
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(VHAT_INSTALL_SERVER_SERVICE)
  configure_file("../service/vhat_server.service.in" "../service/vhat_server.service")

  install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/../service/vhat_server.service"
    DESTINATION "/lib/systemd/system/"
  )
endif(VHAT_INSTALL_SERVER_SERVICE)
