cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

add_library(client_cli SHARED
  ate_client_cli.cpp
  assembly_info.cpp
  converters.cpp
  object.cpp
  wildcard.cpp
)

target_include_directories(client_cli PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/../../include
  ${CMAKE_CURRENT_SOURCE_DIR}/../../include/client_cli
)

target_link_libraries(client_cli
  PRIVATE
    ate_client_lib
)

set_target_properties(client_cli PROPERTIES
  OUTPUT_NAME "ATE"
  COMMON_LANGUAGE_RUNTIME ""
)

if(MSVC)
  # Enable XML documentation but supress warning C4634 for it
  target_compile_options(client_cli PRIVATE "/doc" "/wd4634")
endif()
