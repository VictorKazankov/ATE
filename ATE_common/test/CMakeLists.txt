cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

set(VHAT_COMMON_TEST_DATA_PATH "${CMAKE_CURRENT_SOURCE_DIR}/data")

add_executable(vhat_common_test
  common/common.cpp
  config/config.cpp
  logger/logger.cpp
  message_factory/request.cpp
  utils/geometry_types.cpp
)

target_link_libraries(vhat_common_test
  PRIVATE
    gtest_main
    stdc++fs
    vhat_common
)

target_compile_definitions(vhat_common_test
  PRIVATE
    VHAT_COMMON_TEST_DATA_PATH="${VHAT_COMMON_TEST_DATA_PATH}"
)

# Add vhat_server_test to the CTest
# TODO: replace add_test with gtest_discover_tests after upgrade minimum CMake version to 3.10
# https://cmake.org/cmake/help/latest/module/GoogleTest.html
add_test(
  NAME VHAT_Common_Tests
  COMMAND vhat_common_test
)
