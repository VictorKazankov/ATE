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
    ../include
)

target_link_libraries(ate_client_lib
  PUBLIC
    ate_common
)

target_compile_definitions(ate_client_lib
  PUBLIC
    ATE_CLIENT_CONFIG_DIR="${ATE_INSTALL_SYSCONFDIR}"
)

pybind11_add_module(ate_client ate_client.cpp)

target_include_directories(ate_client
  PUBLIC
    ../include
)

target_link_libraries(ate_client
  PRIVATE
    ate_client_lib
)

set_target_properties(ate_client PROPERTIES
  OUTPUT_NAME "vhat_client"
  INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${ATE_INSTALL_LIBDIR}"
)

install(FILES
  ../data/ate_client.ini
  DESTINATION ${ATE_INSTALL_SYSCONFDIR}
)

if(UNIX)
  message("INFO: ATE client install path was hardcoded for Python 2.7 and Debian system "
          "(https://wiki.debian.org/Python#Deviations_from_upstream)")

  install(TARGETS
    ate_client
    LIBRARY DESTINATION "${ATE_INSTALL_LIBPREFIX}/python2.7/dist-packages"
  )
elseif(WIN32)
  install(TARGETS
    ate_client
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  )
endif()
