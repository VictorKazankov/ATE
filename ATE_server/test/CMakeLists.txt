cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

set(VHAT_SERVER_TEST_DATA_PATH "${CMAKE_CURRENT_SOURCE_DIR}/data")
set(TEST_NAME vhat_server_test)

add_executable_from_dir_recursive(${TEST_NAME})

target_link_libraries(${TEST_NAME}
  PRIVATE
    vhat_server_lib
    ${GTEST_BOTH_LIBRARIES}
    ${GMOCK_BOTH_LIBRARIES}
)

target_compile_definitions(${TEST_NAME}
  PRIVATE
    VHAT_SERVER_TEST_DATA_PATH="${VHAT_SERVER_TEST_DATA_PATH}"
)

gtest_add_tests(${TEST_NAME} "" AUTO)
