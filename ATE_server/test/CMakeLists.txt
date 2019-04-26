cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

set(VHAT_TEST_DATA_PATH "${VHAT_DATA_PATH}/test" CACHE PATH "Path to VHAT test data")

add_executable(vhat_server_test
  config/config.cpp
  storage/json_storage.cpp
)

target_compile_definitions(vhat_server_test
  PRIVATE
    VHAT_TEST_DATA_PATH="${VHAT_TEST_DATA_PATH}"
)

target_link_libraries(vhat_server_test
  PRIVATE
    gtest_main
    vhat_server_lib
)
