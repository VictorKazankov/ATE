cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

set(VHAT_SERVER_TEST_DATA_PATH "${CMAKE_CURRENT_SOURCE_DIR}/data")

# Workaround for build project while some packages found with pkg-config
# TODO: Use IMPORTED_TARGET for pkg_check_modules since CMake 3.6
link_directories(
  "${GStreamer_LIBRARY_DIRS}"
  "${Tesseract_LIBRARY_DIRS}"
)

add_executable(vhat_server_test
  storage/json_storage.cpp
  utils/file_descriptor.cpp
  utils/safe_env.cpp
  video_streaming/matching/hausdorff_distance.cpp
  video_streaming/matching/template_detector.cpp
  video_streaming/matching/text_detector.cpp
  interaction/SPI/spi_interaction.cpp
)

target_compile_definitions(vhat_server_test
  PRIVATE
    VHAT_SERVER_TEST_DATA_PATH="${VHAT_SERVER_TEST_DATA_PATH}"
)

target_link_libraries(vhat_server_test
  PRIVATE
    gtest_main
    vhat_server_lib
)

# Add vhat_server_test to the CTest
# TODO: replace add_test with gtest_discover_tests after upgrade minimum CMake version to 3.10
# https://cmake.org/cmake/help/latest/module/GoogleTest.html
add_test(
  NAME VHAT_Server_Tests
  COMMAND vhat_server_test
)
