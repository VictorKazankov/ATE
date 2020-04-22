cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

add_library(ate_client_lib STATIC
  api.cpp
  api_aggregator.cpp
  ate_api.cpp 
  ate_interaction.cpp
  json_rpc_parser.cpp
  squish/application_context.cpp
  squish/squish_API.cpp
  squish/wildcard.cpp
)

set_target_properties(ate_client_lib PROPERTIES
  POSITION_INDEPENDENT_CODE ON
)

target_include_directories(ate_client_lib
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

target_link_libraries(ate_client_lib
  PUBLIC
    ate_common
)

target_compile_definitions(ate_client_lib
  PUBLIC
    ATE_CLIENT_CONFIG_DIR="${ATE_INSTALL_SYSCONFDIR}"
)

if(MSVC)
  target_compile_definitions(ate_client_lib
    PRIVATE
      _CRT_SECURE_NO_WARNINGS
  )
endif()

install(FILES
  ../data/ate_client.ini
  DESTINATION ${ATE_INSTALL_SYSCONFDIR}
)

if(ATE_BUILD_CLIENT_PYTHON)
  add_subdirectory(client_python)
endif()

if(ATE_BUILD_CLIENT_CLI)
  add_subdirectory(client_cli)
endif()
