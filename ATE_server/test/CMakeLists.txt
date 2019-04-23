cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

add_executable(vhat_server_test
  config/config.cpp
  storage/json_storage.cpp
)

target_link_libraries(vhat_server_test
  PRIVATE
    gtest_main
    vhat_server_lib
)

file(COPY
  config/bad_comment.ini
  config/test_config.ini
  DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/config"
)
