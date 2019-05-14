cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

if(NOT EXISTS "${CMAKE_SOURCE_DIR}/ATE_server/data/test")
  message(FATAL_ERROR "cannot build tests: ATE_server/data/test does not exist")
endif()

set(VHAT_SERVER_TEST_DATA_PATH "${VHAT_SERVER_DATA_PATH}/test" CACHE PATH "Path to VHAT server test data")

add_executable(vhat_server_test
  storage/json_storage.cpp
  utils/safe_env.cpp
  video_streaming/matching/hausdorff_distance.cpp
  video_streaming/matching/template_detector.cpp
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
