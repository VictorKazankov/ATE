cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

if(NOT EXISTS "${CMAKE_SOURCE_DIR}/ATE_common/data/test")
  message(FATAL_ERROR "cannot build tests: ATE_common/data/test does not exist")
endif()

set(VHAT_COMMON_TEST_DATA_PATH "${VHAT_COMMON_DATA_PATH}/test" CACHE PATH "Path to VHAT common test data")

add_executable(vhat_common_test
  config/config.cpp
)

target_link_libraries(vhat_common_test
  PRIVATE
    gtest_main
    vhat_common
)

target_compile_definitions(vhat_common_test
  PRIVATE
    VHAT_COMMON_TEST_DATA_PATH="${VHAT_COMMON_TEST_DATA_PATH}"
)
