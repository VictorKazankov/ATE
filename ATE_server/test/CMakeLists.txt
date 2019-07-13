cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

set(VHAT_SERVER_TEST_DATA_PATH "${CMAKE_CURRENT_SOURCE_DIR}/data")
set(TEST_NAME vhat_server_test)

# Workaround for build project while some packages found with pkg-config
# TODO: Use IMPORTED_TARGET for pkg_check_modules since CMake 3.6
link_directories(
  "${GStreamer_LIBRARY_DIRS}"
  "${Tesseract_LIBRARY_DIRS}"
)

add_executable_from_dir_recursive(${TEST_NAME})

target_link_libraries(${TEST_NAME}
  PRIVATE
    ${GTEST_BOTH_LIBRARIES}
    vhat_server_lib
)

target_compile_definitions(${TEST_NAME}
  PRIVATE
    VHAT_SERVER_TEST_DATA_PATH="${VHAT_SERVER_TEST_DATA_PATH}"
)

gtest_add_tests(${TEST_NAME} "" AUTO)
