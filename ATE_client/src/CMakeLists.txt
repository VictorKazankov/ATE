cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# A separate static library is necessary to support unit tests
add_library(vhat_client_lib
  STATIC
    ate_interaction.cpp
)

target_include_directories(vhat_client_lib
  PUBLIC
    ../include
)

target_link_libraries(vhat_client_lib
  vhat_common
)

add_executable(vhat_client
  ate_client_main.cpp
)

target_compile_definitions(vhat_client_lib
  PUBLIC
    VHAT_CLIENT_DATA_PATH="${VHAT_CLIENT_DATA_PATH}"
)

target_link_libraries(vhat_client
  PRIVATE
    vhat_client_lib
)