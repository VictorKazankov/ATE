cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

include(GNUInstallDirs)

# A separate static library is necessary to support unit tests
add_library(vhat_server_lib
  STATIC
    interaction/dummy/dummy_interaction.cpp
    interaction/VDP/vdp_interaction.cpp
    interaction/VDP/https_client.cpp
    interaction/ATE/tcp_connection.cpp
    interaction/ATE/tcp_connection_manager.cpp
    interaction/ATE/tcp_transport_adapter.cpp
    interaction/SPI/spi_interaction.cpp
    interaction/DBUS/dbus_connection_manager.cpp
    utils/file_descriptor.cpp
    utils/screenshot_recorder.cpp
    video_streaming/matching/matcher.cpp
    video_streaming/v4l_streamer.cpp
    video_streaming/framebuf_to_mat.cpp
    video_streaming/gst_streamer.cpp
    video_streaming/stream_reader.cpp
    video_streaming/streamer_factory.cpp
    transport_adapters_collection.cpp
    ate_message_adapter.cpp
    ate.cpp
)

# DBManager (https://github.ford.com/VHAT/DBManagerLib)
find_package(DBManager ${PROJECT_VERSION} REQUIRED)
if(DBManager_FOUND)
  message(STATUS "Found DBManager library version " ${DBManager_VERSION})
endif()

# pkg-config
# https://cmake.org/cmake/help/latest/module/FindPkgConfig.html
# https://www.freedesktop.org/wiki/Software/pkg-config
find_package(PkgConfig REQUIRED)

# TODO: use pkg-config on LVDS board as well as soon as it will be fixed in vdp-gstreamer package
if (${CMAKE_SYSTEM_PROCESSOR} STREQUAL aarch64)
  # GStreamer custom build for VDP
  # It comes preinstalled on LVDS board
  # https://github.ford.com/LVDS2Eth/Jetson_Gstreamer14

  if (EXISTS "/usr/lib/aarch64-linux-gnu/vdp-gstreamer")
    # Paths installed by vdp-gstreamer package
    set(GStreamer_INCLUDE_DIRS "/usr/include/aarch64-linux-gnu/vdp-gstreamer/gstreamer-1.0")
    set(GStreamer_LIBRARY_DIRS "/usr/lib/aarch64-linux-gnu/vdp-gstreamer")
  elseif(EXISTS "/home/nvidia/gstreamer114_build")
    # gstreamer 1.14 pacakge sewed into Jetson Update package in earlier VDP releases
    set(GStreamer_INCLUDE_DIRS "/home/nvidia/gstreamer114_build/include/gstreamer-1.0")
    set(GStreamer_LIBRARY_DIRS "/home/nvidia/gstreamer114_build/lib")

    # kludge to install legacy systemd file
    set(USE_LEGACY_SYSTEMD_FILE TRUE)
  else()
    message(FATAL_ERROR "GStreamer is expected to be installed into predefined location on LVDS board")
  endif()

  find_library(gstapp NAMES gstapp-1.0 PATHS ${GStreamer_LIBRARY_DIRS} NO_DEFAULT_PATH)
  find_library(gstbase NAMES gstbase-1.0 PATHS ${GStreamer_LIBRARY_DIRS} NO_DEFAULT_PATH)
  find_library(gstreamer NAMES gstreamer-1.0 PATHS ${GStreamer_LIBRARY_DIRS} NO_DEFAULT_PATH)

  if(gstapp AND gstbase AND gstreamer)
    set(GStreamer_LIBRARIES ${gstapp} ${gstbase} ${gstreamer})
    set(GStreamer_FOUND 1)
  endif()
else()
  # GStreamer
  # https://gstreamer.freedesktop.org
  pkg_check_modules(GStreamer REQUIRED
    gstreamer-1.0
    gstreamer-app-1.0
  )
endif()

message(STATUS "Will look for GStreamer headers in: " "${GStreamer_INCLUDE_DIRS}")
message(STATUS "Will look for GStreamer libraries in: " "${GStreamer_LIBRARY_DIRS}")
message(STATUS "Will link to GStreamer libs: " "${GStreamer_LIBRARIES}")

pkg_check_modules(DBus REQUIRED
  dbus-glib-1
  dbus-1
  gobject-2.0
  glib-2.0
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

# Recognition libraries
find_package(Recognition ${PROJECT_VERSION} REQUIRED)
if(Recognition_FOUND)
  message(STATUS "Found Recognition library version " ${Recognition_VERSION})
endif()

# Workaround for build project while some packages found with pkg-config
# TODO: Use IMPORTED_TARGET for pkg_check_modules since CMake 3.6
link_directories(
  "${GStreamer_LIBRARY_DIRS}"
)

target_include_directories(vhat_server_lib
  PUBLIC
    ../include
    "${GStreamer_INCLUDE_DIRS}"
    "${DBus_INCLUDE_DIRS}"
    "${OpenCV_INCLUDE_DIRS}"
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
    VHAT::recognition
    ${GStreamer_LIBRARIES}
    VHAT::dbmanager
    ${DBus_LIBRARIES}
)

add_executable(vhat_server
  ate_server_main.cpp
)

target_compile_definitions(vhat_server_lib
  PUBLIC
    VHAT_SERVER_CONFIG_DIR="${CMAKE_INSTALL_PREFIX}/${VHAT_INSTALL_SYSCONFDIR}"
    VHAT_ICON_STORAGE_PREFIX="${VHAT_ICON_STORAGE_PREFIX}"
    VHAT_WRITABLE_DATA_PREFIX="${VHAT_WRITABLE_DATA_PREFIX}"
)

target_link_libraries(vhat_server
  PRIVATE
    vhat_server_lib
)

# TODO: remove INSTALL_RPATH_USE_LINK_PATH TRUE as soon as disposed of custom GStreamer dependency
set_target_properties(vhat_server PROPERTIES
  INSTALL_RPATH "\$ORIGIN/../${VHAT_INSTALL_LIBDIR}"
  INSTALL_RPATH_USE_LINK_PATH TRUE
)

install(TARGETS
  vhat_server
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(FILES
  ../data/vhat_server.ini
  DESTINATION ${VHAT_INSTALL_SYSCONFDIR}
)

if(VHAT_INSTALL_ICON_STORAGE)
  install(DIRECTORY
    ../data/icon_storage/
    DESTINATION "${VHAT_ICON_STORAGE_PREFIX}"
  )
endif(VHAT_INSTALL_ICON_STORAGE)

if(VHAT_INSTALL_SERVER_SERVICE)
  if(USE_LEGACY_SYSTEMD_FILE)
    configure_file("../service/vhat_server.service.legacy.in" "../service/vhat_server.service")
  else()
    configure_file("../service/vhat_server.service.in" "../service/vhat_server.service")
  endif()

  install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/../service/vhat_server.service"
    DESTINATION "/lib/systemd/system/"
  )
endif(VHAT_INSTALL_SERVER_SERVICE)

#######################################################
#                   Test coverage
#######################################################
if (VHAT_COVERAGE)
  target_compile_options(vhat_server_lib PRIVATE --coverage)
  target_link_libraries(vhat_server_lib PRIVATE gcov)
endif (VHAT_COVERAGE)
