cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(vhat_client)

add_library(vhat_client
  MODULE
    ate_interaction.cpp
    vhat_client.cpp
    squish/application_context.cpp
    squish/squish_API.cpp
)

set_target_properties(vhat_client PROPERTIES
  PREFIX "${PYTHON_MODULE_PREFIX}"
  SUFFIX "${PYTHON_MODULE_EXTENSION}")

target_include_directories(vhat_client
  PUBLIC
    ../include
)

target_link_libraries(vhat_client
  vhat_common
  pybind11::module
)

target_compile_definitions(vhat_client
  PUBLIC
    VHAT_CLIENT_DATA_PATH="${VHAT_CLIENT_DATA_PATH}"
)
